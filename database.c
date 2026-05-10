#include "database.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int exec_sql(sqlite3 *db, const char *sql) {
    char *error_message = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &error_message);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQLite error: %s\n", error_message != NULL ? error_message : "unknown");
        sqlite3_free(error_message);
    }
    return rc;
}

static void copy_text_column(char *dest, size_t size, sqlite3_stmt *stmt, int index) {
    const unsigned char *text = sqlite3_column_text(stmt, index);
    if (text == NULL) {
        dest[0] = '\0';
        return;
    }
    snprintf(dest, size, "%s", (const char *)text);
}

static int db_log_operation(sqlite3 *db, const char *operation, const char *entity, int entity_id, const char *details) {
    sqlite3_stmt *stmt = NULL;
    int rc;

    static const char *sql =
        "INSERT INTO audit_log (operation, entity, entity_id, details) VALUES (?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }

    sqlite3_bind_text(stmt, 1, operation, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, entity, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, entity_id);
    sqlite3_bind_text(stmt, 4, details != NULL ? details : "", -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE ? SQLITE_OK : rc;
}

static const char *sort_clause(SortType sort_type) {
    switch (sort_type) {
        case SORT_BY_RISK:
            return "risk_percentage DESC, ime ASC, prezime ASC";
        case SORT_BY_DATE:
            return "created_at DESC";
        case SORT_BY_NAME:
        default:
            return "ime ASC, prezime ASC";
    }
}

static void bind_query_params(sqlite3_stmt *stmt, const char *search_term, SearchType search_type) {
    char like_buffer[256];
    int id_value;

    switch (search_type) {
        case SEARCH_BY_ID:
            id_value = atoi(search_term);
            sqlite3_bind_int(stmt, 1, id_value);
            break;
        case SEARCH_BY_NAME:
            snprintf(like_buffer, sizeof(like_buffer), "%%%s%%", search_term);
            sqlite3_bind_text(stmt, 1, like_buffer, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, like_buffer, -1, SQLITE_TRANSIENT);
            break;
        case SEARCH_BY_TYPE:
            snprintf(like_buffer, sizeof(like_buffer), "%%%s%%", search_term);
            sqlite3_bind_text(stmt, 1, like_buffer, -1, SQLITE_TRANSIENT);
            break;
        case SEARCH_NONE:
        default:
            break;
    }
}

int db_open(sqlite3 **db, const char *path) {
    int rc = sqlite3_open(path, db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Neuspjelo otvaranje baze: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    return exec_sql(*db, "PRAGMA foreign_keys = ON;");
}

int db_initialize(sqlite3 *db) {
    const char *schema =
        "CREATE TABLE IF NOT EXISTS pacijenti ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "ime TEXT NOT NULL,"
        "prezime TEXT NOT NULL,"
        "dob INTEGER NOT NULL CHECK(dob >= 0 AND dob <= 150),"
        "screening_type TEXT NOT NULL,"
        "risk_percentage REAL NOT NULL CHECK(risk_percentage >= 0 AND risk_percentage <= 100),"
        "email TEXT,"
        "phone TEXT,"
        "created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,"
        "updated_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP"
        ");"
        "CREATE TABLE IF NOT EXISTS pracenje ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "pacijent_id INTEGER NOT NULL,"
        "note TEXT NOT NULL,"
        "created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY(pacijent_id) REFERENCES pacijenti(id) ON DELETE CASCADE"
        ");"
        "CREATE TABLE IF NOT EXISTS audit_log ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "operation TEXT NOT NULL,"
        "entity TEXT NOT NULL,"
        "entity_id INTEGER NOT NULL,"
        "details TEXT,"
        "created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP"
        ");";

    return exec_sql(db, schema);
}

void db_close(sqlite3 *db) {
    if (db != NULL) {
        sqlite3_close(db);
    }
}

int db_add_patient(sqlite3 *db, const Patient *patient, int *new_id) {
    sqlite3_stmt *stmt = NULL;
    int rc;
    static const char *sql =
        "INSERT INTO pacijenti (ime, prezime, dob, screening_type, risk_percentage, email, phone) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }

    sqlite3_bind_text(stmt, 1, patient->ime, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, patient->prezime, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, patient->dob);
    sqlite3_bind_text(stmt, 4, patient->screening_type, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, patient->risk_percentage);
    sqlite3_bind_text(stmt, 6, patient->email, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, patient->phone, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        return rc;
    }

    if (new_id != NULL) {
        *new_id = (int)sqlite3_last_insert_rowid(db);
    }
    db_log_operation(db, "CREATE", "pacijenti", (int)sqlite3_last_insert_rowid(db), "Novi pacijent dodan");
    return SQLITE_OK;
}

int db_get_patient(sqlite3 *db, int id, Patient *patient) {
    sqlite3_stmt *stmt = NULL;
    int rc;
    static const char *sql =
        "SELECT id, ime, prezime, dob, screening_type, risk_percentage, email, phone, created_at, updated_at "
        "FROM pacijenti WHERE id = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }

    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        patient->id = sqlite3_column_int(stmt, 0);
        copy_text_column(patient->ime, sizeof(patient->ime), stmt, 1);
        copy_text_column(patient->prezime, sizeof(patient->prezime), stmt, 2);
        patient->dob = sqlite3_column_int(stmt, 3);
        copy_text_column(patient->screening_type, sizeof(patient->screening_type), stmt, 4);
        patient->risk_percentage = sqlite3_column_double(stmt, 5);
        copy_text_column(patient->email, sizeof(patient->email), stmt, 6);
        copy_text_column(patient->phone, sizeof(patient->phone), stmt, 7);
        copy_text_column(patient->created_at, sizeof(patient->created_at), stmt, 8);
        copy_text_column(patient->updated_at, sizeof(patient->updated_at), stmt, 9);
        sqlite3_finalize(stmt);
        return SQLITE_OK;
    }

    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE ? SQLITE_NOTFOUND : rc;
}

int db_update_patient(sqlite3 *db, const Patient *patient) {
    sqlite3_stmt *stmt = NULL;
    int rc;
    static const char *sql =
        "UPDATE pacijenti SET ime = ?, prezime = ?, dob = ?, screening_type = ?, risk_percentage = ?, "
        "email = ?, phone = ?, updated_at = CURRENT_TIMESTAMP WHERE id = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }

    sqlite3_bind_text(stmt, 1, patient->ime, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, patient->prezime, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, patient->dob);
    sqlite3_bind_text(stmt, 4, patient->screening_type, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, patient->risk_percentage);
    sqlite3_bind_text(stmt, 6, patient->email, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, patient->phone, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 8, patient->id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        return rc;
    }
    if (sqlite3_changes(db) == 0) {
        return SQLITE_NOTFOUND;
    }

    db_log_operation(db, "UPDATE", "pacijenti", patient->id, "Azuriran zapis pacijenta");
    return SQLITE_OK;
}

int db_delete_patient(sqlite3 *db, int id) {
    sqlite3_stmt *stmt = NULL;
    int rc;
    static const char *sql = "DELETE FROM pacijenti WHERE id = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }

    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        return rc;
    }
    if (sqlite3_changes(db) == 0) {
        return SQLITE_NOTFOUND;
    }

    db_log_operation(db, "DELETE", "pacijenti", id, "Obrisan zapis pacijenta");
    return SQLITE_OK;
}

int db_restore_patient(sqlite3 *db, const Patient *patient) {
    sqlite3_stmt *stmt = NULL;
    int rc;
    static const char *sql =
        "INSERT INTO pacijenti (id, ime, prezime, dob, screening_type, risk_percentage, email, phone, created_at, updated_at) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }

    sqlite3_bind_int(stmt, 1, patient->id);
    sqlite3_bind_text(stmt, 2, patient->ime, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, patient->prezime, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, patient->dob);
    sqlite3_bind_text(stmt, 5, patient->screening_type, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 6, patient->risk_percentage);
    sqlite3_bind_text(stmt, 7, patient->email, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, patient->phone, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, patient->created_at, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 10, patient->updated_at, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        return rc;
    }

    db_log_operation(db, "RESTORE", "pacijenti", patient->id, "Undo brisanja pacijenta");
    return SQLITE_OK;
}

int db_add_followup(sqlite3 *db, int patient_id, const char *note) {
    sqlite3_stmt *stmt = NULL;
    int rc;
    static const char *sql = "INSERT INTO pracenje (pacijent_id, note) VALUES (?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }

    sqlite3_bind_int(stmt, 1, patient_id);
    sqlite3_bind_text(stmt, 2, note, -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        return rc;
    }

    db_log_operation(db, "CREATE", "pracenje", patient_id, note);
    return SQLITE_OK;
}

int db_query_patients(sqlite3 *db, const char *search_term, SearchType search_type, SortType sort_type,
                      int page, int page_size, Patient **out_patients, int *out_count, int *out_total) {
    const char *where = "";
    char count_sql[512];
    char data_sql[512];
    sqlite3_stmt *count_stmt = NULL;
    sqlite3_stmt *data_stmt = NULL;
    int rc;
    int offset = (page - 1) * page_size;
    Patient *patients = NULL;
    int count = 0;
    int capacity = page_size > 0 ? page_size : 1;

    switch (search_type) {
        case SEARCH_BY_ID:
            where = " WHERE id = ?";
            break;
        case SEARCH_BY_NAME:
            where = " WHERE ime LIKE ? OR prezime LIKE ?";
            break;
        case SEARCH_BY_TYPE:
            where = " WHERE screening_type LIKE ?";
            break;
        case SEARCH_NONE:
        default:
            where = "";
            break;
    }

    snprintf(count_sql, sizeof(count_sql), "SELECT COUNT(*) FROM pacijenti%s;", where);
    rc = sqlite3_prepare_v2(db, count_sql, -1, &count_stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }
    bind_query_params(count_stmt, search_term != NULL ? search_term : "", search_type);
    rc = sqlite3_step(count_stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(count_stmt);
        return rc;
    }
    *out_total = sqlite3_column_int(count_stmt, 0);
    sqlite3_finalize(count_stmt);

    snprintf(data_sql, sizeof(data_sql),
             "SELECT id, ime, prezime, dob, screening_type, risk_percentage, email, phone, created_at, updated_at "
             "FROM pacijenti%s ORDER BY %s LIMIT ? OFFSET ?;",
             where, sort_clause(sort_type));

    rc = sqlite3_prepare_v2(db, data_sql, -1, &data_stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }
    bind_query_params(data_stmt, search_term != NULL ? search_term : "", search_type);

    switch (search_type) {
        case SEARCH_BY_ID:
        case SEARCH_BY_TYPE:
            sqlite3_bind_int(data_stmt, 2, page_size);
            sqlite3_bind_int(data_stmt, 3, offset);
            break;
        case SEARCH_BY_NAME:
            sqlite3_bind_int(data_stmt, 3, page_size);
            sqlite3_bind_int(data_stmt, 4, offset);
            break;
        case SEARCH_NONE:
        default:
            sqlite3_bind_int(data_stmt, 1, page_size);
            sqlite3_bind_int(data_stmt, 2, offset);
            break;
    }

    patients = (Patient *)calloc((size_t)capacity, sizeof(Patient));
    if (patients == NULL) {
        sqlite3_finalize(data_stmt);
        return SQLITE_NOMEM;
    }

    while ((rc = sqlite3_step(data_stmt)) == SQLITE_ROW) {
        if (count >= capacity) {
            Patient *resized;
            capacity *= 2;
            resized = (Patient *)realloc(patients, (size_t)capacity * sizeof(Patient));
            if (resized == NULL) {
                free(patients);
                sqlite3_finalize(data_stmt);
                return SQLITE_NOMEM;
            }
            patients = resized;
        }
        patients[count].id = sqlite3_column_int(data_stmt, 0);
        copy_text_column(patients[count].ime, sizeof(patients[count].ime), data_stmt, 1);
        copy_text_column(patients[count].prezime, sizeof(patients[count].prezime), data_stmt, 2);
        patients[count].dob = sqlite3_column_int(data_stmt, 3);
        copy_text_column(patients[count].screening_type, sizeof(patients[count].screening_type), data_stmt, 4);
        patients[count].risk_percentage = sqlite3_column_double(data_stmt, 5);
        copy_text_column(patients[count].email, sizeof(patients[count].email), data_stmt, 6);
        copy_text_column(patients[count].phone, sizeof(patients[count].phone), data_stmt, 7);
        copy_text_column(patients[count].created_at, sizeof(patients[count].created_at), data_stmt, 8);
        copy_text_column(patients[count].updated_at, sizeof(patients[count].updated_at), data_stmt, 9);
        count++;
    }

    sqlite3_finalize(data_stmt);
    if (rc != SQLITE_DONE) {
        free(patients);
        return rc;
    }

    *out_patients = patients;
    *out_count = count;
    return SQLITE_OK;
}

void db_free_patients(Patient *patients) {
    free(patients);
}

int db_get_statistics(sqlite3 *db, ScreeningStats **out_stats, int *out_count, int *out_total_patients) {
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *total_stmt = NULL;
    ScreeningStats *stats = NULL;
    int count = 0;
    int capacity = 8;
    int rc;
    static const char *sql =
        "SELECT screening_type, COUNT(*), AVG(risk_percentage) "
        "FROM pacijenti GROUP BY screening_type ORDER BY screening_type;";

    rc = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM pacijenti;", -1, &total_stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }
    rc = sqlite3_step(total_stmt);
    if (rc == SQLITE_ROW) {
        *out_total_patients = sqlite3_column_int(total_stmt, 0);
    } else {
        sqlite3_finalize(total_stmt);
        return rc;
    }
    sqlite3_finalize(total_stmt);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }

    stats = (ScreeningStats *)calloc((size_t)capacity, sizeof(ScreeningStats));
    if (stats == NULL) {
        sqlite3_finalize(stmt);
        return SQLITE_NOMEM;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (count >= capacity) {
            ScreeningStats *resized;
            capacity *= 2;
            resized = (ScreeningStats *)realloc(stats, (size_t)capacity * sizeof(ScreeningStats));
            if (resized == NULL) {
                free(stats);
                sqlite3_finalize(stmt);
                return SQLITE_NOMEM;
            }
            stats = resized;
        }
        copy_text_column(stats[count].screening_type, sizeof(stats[count].screening_type), stmt, 0);
        stats[count].patient_count = sqlite3_column_int(stmt, 1);
        stats[count].avg_risk = sqlite3_column_double(stmt, 2);
        count++;
    }

    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        free(stats);
        return rc;
    }

    *out_stats = stats;
    *out_count = count;
    return SQLITE_OK;
}

void db_free_stats(ScreeningStats *stats) {
    free(stats);
}

static void csv_escape(FILE *file, const char *value) {
    const char *cursor = value != NULL ? value : "";
    fputc('"', file);
    while (*cursor != '\0') {
        if (*cursor == '"') {
            fputc('"', file);
        }
        fputc(*cursor, file);
        cursor++;
    }
    fputc('"', file);
}

int db_export_csv(sqlite3 *db, const char *file_path) {
    sqlite3_stmt *stmt = NULL;
    FILE *file;
    int rc;
    static const char *sql =
        "SELECT id, ime, prezime, dob, screening_type, risk_percentage, email, phone, created_at, updated_at "
        "FROM pacijenti ORDER BY id ASC;";

    file = fopen(file_path, "w");
    if (file == NULL) {
        return SQLITE_CANTOPEN;
    }

    fprintf(file, "id,ime,prezime,dob,screening_type,risk_percentage,email,phone,created_at,updated_at\n");

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fclose(file);
        return rc;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *ime = sqlite3_column_text(stmt, 1);
        const unsigned char *prezime = sqlite3_column_text(stmt, 2);
        const unsigned char *type = sqlite3_column_text(stmt, 4);
        const unsigned char *email = sqlite3_column_text(stmt, 6);
        const unsigned char *phone = sqlite3_column_text(stmt, 7);
        const unsigned char *created = sqlite3_column_text(stmt, 8);
        const unsigned char *updated = sqlite3_column_text(stmt, 9);

        fprintf(file, "%d,", sqlite3_column_int(stmt, 0));
        csv_escape(file, (const char *)ime);
        fputc(',', file);
        csv_escape(file, (const char *)prezime);
        fprintf(file, ",%d,", sqlite3_column_int(stmt, 3));
        csv_escape(file, (const char *)type);
        fprintf(file, ",%.2f,", sqlite3_column_double(stmt, 5));
        csv_escape(file, (const char *)email);
        fputc(',', file);
        csv_escape(file, (const char *)phone);
        fputc(',', file);
        csv_escape(file, (const char *)created);
        fputc(',', file);
        csv_escape(file, (const char *)updated);
        fputc('\n', file);
    }

    sqlite3_finalize(stmt);
    fclose(file);

    if (rc != SQLITE_DONE) {
        return rc;
    }

    db_log_operation(db, "EXPORT", "pacijenti", 0, file_path);
    return SQLITE_OK;
}

int db_backup(sqlite3 *db, const char *backup_path) {
    sqlite3 *backup_db = NULL;
    sqlite3_backup *backup = NULL;
    int rc;

    rc = sqlite3_open(backup_path, &backup_db);
    if (rc != SQLITE_OK) {
        if (backup_db != NULL) {
            sqlite3_close(backup_db);
        }
        return rc;
    }

    backup = sqlite3_backup_init(backup_db, "main", db, "main");
    if (backup == NULL) {
        sqlite3_close(backup_db);
        return sqlite3_errcode(backup_db);
    }

    rc = sqlite3_backup_step(backup, -1);
    sqlite3_backup_finish(backup);

    if (rc == SQLITE_DONE) {
        rc = SQLITE_OK;
    }

    sqlite3_close(backup_db);
    if (rc == SQLITE_OK) {
        db_log_operation(db, "BACKUP", "database", 0, backup_path);
    }
    return rc;
}
