// datoteke.c -- UPRAVLJANJE DATOTEKAMA
#define _CRT_SECURE_NO_WARNINGS
#include "pacijent.h"

// --- SPREMI U DATOTEKU --- (1: CREATE - datoteka, 19: fopen/fclose)
void spremi_u_datoteku(const Pacijent* p) {
    if (p == NULL) return; // (14: zaštita)

    FILE* fp = fopen("kartoni.txt", "a"); // (19: fopen)
    if (fp == NULL) { // (19: provjera)
        printf(" [!] Greska pri otvaranju datoteke!\n");
        perror("fopen"); // (22: perror)
        return;
    }

    fprintf(fp, "%-12s %-12s %-4d %-15s %-7.2f%% %-30s\n",
        p->ime, p->prezime, p->godine, p->tip_pregleda,
        p->rizik_postotak, p->dijagnoza);

    fclose(fp); // (19: fclose)
}

// --- ČITAJ PACIJENTE IZ DATOTEKE --- (1: READ - datoteka, 19: fopen/fclose)
int ucitaj_pacijente_iz_datoteke(PacijentList* lista) {
    if (lista == NULL) return 0; // (14)

    FILE* fp = fopen("kartoni.txt", "r"); // (19: fopen)
    if (fp == NULL) { // (19: provjera)
        printf(" Datoteka ne postoji ili je prazna.\n");
        return 1;
    }

    Pacijent temp_pacijent;
    while (fscanf(fp, "%49s %49s %d %29s %f %99s",
        temp_pacijent.ime,
        temp_pacijent.prezime,
        &temp_pacijent.godine,
        temp_pacijent.tip_pregleda,
        &temp_pacijent.rizik_postotak,
        temp_pacijent.dijagnoza) == 6) {

        if (!dodaj_pacijenta_u_listu(lista, &temp_pacijent)) {
            fclose(fp); // (19)
            return 0;
        }

        dodaj_u_povezanu_listu(&dvostuka_lista, &temp_pacijent);
    }

    fclose(fp); // (19: fclose)
    return 1;
}