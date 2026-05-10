#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

typedef struct {
    int id;
    char ime[64];
    char prezime[64];
    int dob;
    char screening_type[64];
    double risk_percentage;
    char email[128];
    char phone[32];
    char created_at[32];
    char updated_at[32];
} Patient;

typedef enum {
    SEARCH_NONE = 0,
    SEARCH_BY_ID,
    SEARCH_BY_NAME,
    SEARCH_BY_TYPE
} SearchType;

typedef enum {
    SORT_BY_NAME = 0,
    SORT_BY_RISK,
    SORT_BY_DATE
} SortType;

typedef struct {
    char screening_type[64];
    int patient_count;
    double avg_risk;
} ScreeningStats;

int db_open(sqlite3 **db, const char *path);
int db_initialize(sqlite3 *db);
void db_close(sqlite3 *db);

int db_add_patient(sqlite3 *db, const Patient *patient, int *new_id);
int db_get_patient(sqlite3 *db, int id, Patient *patient);
int db_update_patient(sqlite3 *db, const Patient *patient);
int db_delete_patient(sqlite3 *db, int id);
int db_restore_patient(sqlite3 *db, const Patient *patient);

int db_add_followup(sqlite3 *db, int patient_id, const char *note);
int db_query_patients(sqlite3 *db, const char *search_term, SearchType search_type, SortType sort_type,
                      int page, int page_size, Patient **out_patients, int *out_count, int *out_total);
void db_free_patients(Patient *patients);

int db_get_statistics(sqlite3 *db, ScreeningStats **out_stats, int *out_count, int *out_total_patients);
void db_free_stats(ScreeningStats *stats);

int db_export_csv(sqlite3 *db, const char *file_path);
int db_backup(sqlite3 *db, const char *backup_path);

#endif
