#include "database.h"
#include "ui.h"
#include "utils.h"

#include <sqlite3.h>

int main(void) {
    sqlite3 *db = NULL;
    int rc;

    rc = db_open(&db, "onkoc.db");
    if (rc != SQLITE_OK) {
        utils_print_error("Neuspjelo otvaranje baze podataka.");
        return 1;
    }

    rc = db_initialize(db);
    if (rc != SQLITE_OK) {
        utils_print_error("Neuspjela inicijalizacija baze podataka.");
        db_close(db);
        return 1;
    }

    utils_print_success("Baza podataka je spremna.");
    rc = ui_run(db);
    db_close(db);
    return rc;
}
