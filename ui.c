#include "ui.h"

#include "database.h"
#include "utils.h"
#include "validation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int has_deleted;
    Patient deleted_patient;
} UndoState;

static UndoState undo_state = {0};

static void print_menu(void) {
    printf("\n===== OnkoC Medical Screening =====\n");
    printf("1. Dodaj novog pacijenta\n");
    printf("2. Pretraga i prikaz pacijenata\n");
    printf("3. Uredi pacijenta\n");
    printf("4. Obrisi pacijenta\n");
    printf("5. Dodaj biljesku pracenja\n");
    printf("6. Statistika\n");
    printf("7. Izvoz u CSV\n");
    printf("8. Backup baze\n");
    printf("9. Undo zadnjeg brisanja\n");
    printf("0. Izlaz\n");
    printf("===================================\n");
}

static void print_patient(const Patient *patient) {
    printf("ID: %d | %s %s | Dob: %d | Tip: %s | Rizik: %.2f%% | Email: %s | Tel: %s | Kreirano: %s\n",
           patient->id,
           patient->ime,
           patient->prezime,
           patient->dob,
           patient->screening_type,
           patient->risk_percentage,
           patient->email[0] != '\0' ? patient->email : "-",
           patient->phone[0] != '\0' ? patient->phone : "-",
           patient->created_at);
}

static int read_patient_data(Patient *patient) {
    if (!validation_read_non_empty("Ime: ", patient->ime, sizeof(patient->ime))) {
        return 0;
    }
    if (!validation_read_non_empty("Prezime: ", patient->prezime, sizeof(patient->prezime))) {
        return 0;
    }
    if (!validation_read_int_range("Dob (0-150): ", 0, 150, &patient->dob)) {
        return 0;
    }
    if (!validation_read_non_empty("Tip screeninga: ", patient->screening_type, sizeof(patient->screening_type))) {
        return 0;
    }
    if (!validation_read_double_range("Procjena rizika (0-100): ", 0.0, 100.0, &patient->risk_percentage)) {
        return 0;
    }
    if (!validation_read_optional_email("Email (opcionalno): ", patient->email, sizeof(patient->email))) {
        return 0;
    }
    if (!validation_read_optional_phone("Telefon (opcionalno): ", patient->phone, sizeof(patient->phone))) {
        return 0;
    }
    return 1;
}

static void add_patient(sqlite3 *db) {
    Patient patient;
    int rc;
    int new_id = 0;
    char message[128];

    memset(&patient, 0, sizeof(patient));
    if (!read_patient_data(&patient)) {
        utils_print_error("Prekinut unos pacijenta.");
        return;
    }

    rc = db_add_patient(db, &patient, &new_id);
    if (rc != SQLITE_OK) {
        utils_print_error("Neuspjesno spremanje pacijenta.");
        return;
    }

    snprintf(message, sizeof(message), "Pacijent uspjesno dodan. ID: %d", new_id);
    utils_print_success(message);
}

static SearchType read_search_type(void) {
    int choice;
    printf("\nPretraga:\n");
    printf("1. Svi pacijenti\n");
    printf("2. Po ID-u\n");
    printf("3. Po imenu/prezimenu\n");
    printf("4. Po tipu screeninga\n");
    if (!validation_read_int_range("Odabir: ", 1, 4, &choice)) {
        return SEARCH_NONE;
    }
    switch (choice) {
        case 2:
            return SEARCH_BY_ID;
        case 3:
            return SEARCH_BY_NAME;
        case 4:
            return SEARCH_BY_TYPE;
        case 1:
        default:
            return SEARCH_NONE;
    }
}

static SortType read_sort_type(void) {
    int choice;
    printf("\nSortiranje:\n");
    printf("1. Po imenu\n");
    printf("2. Po riziku\n");
    printf("3. Po datumu unosa\n");
    if (!validation_read_int_range("Odabir: ", 1, 3, &choice)) {
        return SORT_BY_NAME;
    }
    switch (choice) {
        case 2:
            return SORT_BY_RISK;
        case 3:
            return SORT_BY_DATE;
        case 1:
        default:
            return SORT_BY_NAME;
    }
}

static void list_patients(sqlite3 *db) {
    SearchType search_type = read_search_type();
    SortType sort_type = read_sort_type();
    char search_term[128] = "";
    char command[16];
    int page = 1;
    const int page_size = 5;

    if (search_type != SEARCH_NONE) {
        if (!validation_read_non_empty("Unesite pojam pretrage: ", search_term, sizeof(search_term))) {
            utils_print_error("Prekinuta pretraga.");
            return;
        }
    }

    while (1) {
        Patient *patients = NULL;
        int count = 0;
        int total = 0;
        int rc = db_query_patients(db, search_term, search_type, sort_type, page, page_size, &patients, &count, &total);
        int total_pages;

        if (rc != SQLITE_OK) {
            utils_print_error("Greska pri dohvat-u pacijenata.");
            return;
        }

        if (total == 0) {
            utils_print_info("Nema rezultata za trazene kriterije.");
            db_free_patients(patients);
            return;
        }

        total_pages = (total + page_size - 1) / page_size;
        printf("\n--- Stranica %d/%d (ukupno %d) ---\n", page, total_pages, total);
        for (int i = 0; i < count; i++) {
            print_patient(&patients[i]);
        }
        db_free_patients(patients);

        if (total_pages == 1) {
            return;
        }

        if (!utils_read_line("Komanda [n=sljedeca, p=prethodna, q=izlaz]: ", command, sizeof(command))) {
            return;
        }
        utils_trim(command);
        if (strcmp(command, "n") == 0 && page < total_pages) {
            page++;
        } else if (strcmp(command, "p") == 0 && page > 1) {
            page--;
        } else if (strcmp(command, "q") == 0) {
            return;
        } else {
            utils_print_error("Nepoznata komanda.");
        }
    }
}

static int read_optional_int(const char *prompt, int min, int max, int current, int *out) {
    char input[64];
    char *end;
    long value;

    if (!utils_read_line(prompt, input, sizeof(input))) {
        return 0;
    }
    utils_trim(input);
    if (input[0] == '\0') {
        *out = current;
        return 1;
    }
    value = strtol(input, &end, 10);
    if (*end != '\0' || value < min || value > max) {
        return 0;
    }
    *out = (int)value;
    return 1;
}

static int read_optional_double(const char *prompt, double min, double max, double current, double *out) {
    char input[64];
    char *end;
    double value;

    if (!utils_read_line(prompt, input, sizeof(input))) {
        return 0;
    }
    utils_trim(input);
    if (input[0] == '\0') {
        *out = current;
        return 1;
    }
    value = strtod(input, &end);
    if (*end != '\0' || value < min || value > max) {
        return 0;
    }
    *out = value;
    return 1;
}

static int read_optional_text(const char *prompt, char *dest, size_t size, const char *current, int mandatory) {
    char input[256];
    if (!utils_read_line(prompt, input, sizeof(input))) {
        return 0;
    }
    utils_trim(input);
    if (input[0] == '\0') {
        snprintf(dest, size, "%s", current);
        return 1;
    }
    if (mandatory && !validation_validate_non_empty(input)) {
        return 0;
    }
    snprintf(dest, size, "%s", input);
    return 1;
}

static void update_patient(sqlite3 *db) {
    int id;
    int rc;
    Patient patient;
    char prompt[256];

    if (!validation_read_int_range("ID pacijenta za izmjenu: ", 1, 1000000000, &id)) {
        return;
    }

    rc = db_get_patient(db, id, &patient);
    if (rc != SQLITE_OK) {
        utils_print_error("Pacijent nije pronaden.");
        return;
    }

    print_patient(&patient);
    utils_print_info("Pritisnite Enter za zadrzavanje postojece vrijednosti.");

    snprintf(prompt, sizeof(prompt), "Ime [%s]: ", patient.ime);
    if (!read_optional_text(prompt, patient.ime, sizeof(patient.ime), patient.ime, 1)) {
        utils_print_error("Neispravno ime.");
        return;
    }
    snprintf(prompt, sizeof(prompt), "Prezime [%s]: ", patient.prezime);
    if (!read_optional_text(prompt, patient.prezime, sizeof(patient.prezime), patient.prezime, 1)) {
        utils_print_error("Neispravno prezime.");
        return;
    }
    snprintf(prompt, sizeof(prompt), "Dob [%d]: ", patient.dob);
    if (!read_optional_int(prompt, 0, 150, patient.dob, &patient.dob)) {
        utils_print_error("Neispravna dob.");
        return;
    }
    snprintf(prompt, sizeof(prompt), "Tip screeninga [%s]: ", patient.screening_type);
    if (!read_optional_text(prompt, patient.screening_type, sizeof(patient.screening_type), patient.screening_type, 1)) {
        utils_print_error("Neispravan tip screeninga.");
        return;
    }
    snprintf(prompt, sizeof(prompt), "Rizik [%.2f]: ", patient.risk_percentage);
    if (!read_optional_double(prompt, 0.0, 100.0, patient.risk_percentage, &patient.risk_percentage)) {
        utils_print_error("Neispravan rizik.");
        return;
    }
    snprintf(prompt, sizeof(prompt), "Email [%s]: ", patient.email[0] != '\0' ? patient.email : "-");
    if (!read_optional_text(prompt, patient.email, sizeof(patient.email), patient.email, 0) ||
        !validation_validate_email(patient.email)) {
        utils_print_error("Neispravan email.");
        return;
    }
    snprintf(prompt, sizeof(prompt), "Telefon [%s]: ", patient.phone[0] != '\0' ? patient.phone : "-");
    if (!read_optional_text(prompt, patient.phone, sizeof(patient.phone), patient.phone, 0) ||
        !validation_validate_phone(patient.phone)) {
        utils_print_error("Neispravan telefon.");
        return;
    }

    if (!utils_confirm("Potvrdi izmjenu")) {
        utils_print_info("Izmjena otkazana.");
        return;
    }

    rc = db_update_patient(db, &patient);
    if (rc == SQLITE_OK) {
        utils_print_success("Pacijent uspjesno azuriran.");
    } else {
        utils_print_error("Azuriranje nije uspjelo.");
    }
}

static void delete_patient(sqlite3 *db) {
    int id;
    int rc;
    Patient patient;

    if (!validation_read_int_range("ID pacijenta za brisanje: ", 1, 1000000000, &id)) {
        return;
    }

    rc = db_get_patient(db, id, &patient);
    if (rc != SQLITE_OK) {
        utils_print_error("Pacijent nije pronaden.");
        return;
    }

    print_patient(&patient);
    if (!utils_confirm("Potvrdi brisanje")) {
        utils_print_info("Brisanje otkazano.");
        return;
    }

    rc = db_delete_patient(db, id);
    if (rc != SQLITE_OK) {
        utils_print_error("Brisanje nije uspjelo.");
        return;
    }

    undo_state.deleted_patient = patient;
    undo_state.has_deleted = 1;
    utils_print_success("Pacijent obrisan. Dostupna je opcija undo.");
}

static void undo_delete(sqlite3 *db) {
    int rc;
    if (!undo_state.has_deleted) {
        utils_print_info("Nema operacije za undo.");
        return;
    }
    if (!utils_confirm("Potvrdi undo zadnjeg brisanja")) {
        utils_print_info("Undo otkazan.");
        return;
    }
    rc = db_restore_patient(db, &undo_state.deleted_patient);
    if (rc == SQLITE_OK) {
        undo_state.has_deleted = 0;
        utils_print_success("Pacijent uspjesno vracen.");
    } else {
        utils_print_error("Undo nije uspio.");
    }
}

static void add_followup(sqlite3 *db) {
    int id;
    char note[512];
    int rc;
    Patient patient;

    if (!validation_read_int_range("ID pacijenta za pracenje: ", 1, 1000000000, &id)) {
        return;
    }
    rc = db_get_patient(db, id, &patient);
    if (rc != SQLITE_OK) {
        utils_print_error("Pacijent nije pronaden.");
        return;
    }
    if (!validation_read_non_empty("Biljeska pracenja: ", note, sizeof(note))) {
        return;
    }
    rc = db_add_followup(db, id, note);
    if (rc == SQLITE_OK) {
        utils_print_success("Biljeska pracenja je spremljena.");
    } else {
        utils_print_error("Spremanje biljeske nije uspjelo.");
    }
}

static void show_statistics(sqlite3 *db) {
    ScreeningStats *stats = NULL;
    int count = 0;
    int total_patients = 0;
    int rc = db_get_statistics(db, &stats, &count, &total_patients);

    if (rc != SQLITE_OK) {
        utils_print_error("Neuspjesno dohvacanje statistike.");
        return;
    }

    printf("\nUkupno pacijenata: %d\n", total_patients);
    if (count == 0) {
        utils_print_info("Nema podataka za statistiku.");
        db_free_stats(stats);
        return;
    }

    printf("%-25s %-12s %-12s\n", "Tip screeninga", "Broj", "Prosjek rizika");
    for (int i = 0; i < count; i++) {
        printf("%-25s %-12d %-12.2f\n", stats[i].screening_type, stats[i].patient_count, stats[i].avg_risk);
    }
    db_free_stats(stats);
}

static void export_csv(sqlite3 *db) {
    char path[256];
    int rc;

    if (!utils_read_line("Putanja CSV datoteke [pacijenti.csv]: ", path, sizeof(path))) {
        return;
    }
    utils_trim(path);
    if (path[0] == '\0') {
        snprintf(path, sizeof(path), "pacijenti.csv");
    }

    rc = db_export_csv(db, path);
    if (rc == SQLITE_OK) {
        char message[320];
        snprintf(message, sizeof(message), "CSV izvoz uspjesan: %s", path);
        utils_print_success(message);
    } else {
        utils_print_error("CSV izvoz nije uspio.");
    }
}

static void backup_database(sqlite3 *db) {
    char path[256];
    int rc;

    utils_default_backup_name(path, sizeof(path));
    printf("Predlozena backup datoteka: %s\n", path);
    if (!utils_read_line("Putanja backup datoteke (Enter za predlozenu): ", path, sizeof(path))) {
        return;
    }
    utils_trim(path);
    if (path[0] == '\0') {
        utils_default_backup_name(path, sizeof(path));
    }

    rc = db_backup(db, path);
    if (rc == SQLITE_OK) {
        char message[320];
        snprintf(message, sizeof(message), "Backup uspjesan: %s", path);
        utils_print_success(message);
    } else {
        utils_print_error("Backup nije uspio.");
    }
}

int ui_run(sqlite3 *db) {
    int choice;
    while (1) {
        print_menu();
        if (!validation_read_int_range("Odaberite opciju: ", 0, 9, &choice)) {
            utils_print_error("Neuspjeh pri citanju opcije.");
            return 1;
        }

        switch (choice) {
            case 1:
                add_patient(db);
                break;
            case 2:
                list_patients(db);
                break;
            case 3:
                update_patient(db);
                break;
            case 4:
                delete_patient(db);
                break;
            case 5:
                add_followup(db);
                break;
            case 6:
                show_statistics(db);
                break;
            case 7:
                export_csv(db);
                break;
            case 8:
                backup_database(db);
                break;
            case 9:
                undo_delete(db);
                break;
            case 0:
                utils_print_info("Zatvaranje aplikacije.");
                return 0;
            default:
                utils_print_error("Nepoznata opcija.");
                break;
        }
    }
}
