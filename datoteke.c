#define _CRT_SECURE_NO_WARNINGS
#include "pacijent.h"

extern PacijentList lista_pacijenata;
extern int dodaj_pacijenta_u_listu(PacijentList* lista, const Pacijent* pacijent);

// SPREMI U DATOTEKU
void spremi_u_datoteku(const Pacijent* p) {
    if (p == NULL) return;

    FILE* fp = fopen("kartoni.txt", "a");
    if (fp == NULL) {
        printf(" [!] Greska pri otvaranju datoteke!\n");
        return;
    }

    fprintf(fp, "%s %s %d %s %.0f %s\n",
        p->ime, p->prezime, p->godine, p->tip_pregleda,
        p->rizik_postotak, p->dijagnoza);

    fclose(fp);
}

// UČITAJ PACIJENTE IZ DATOTEKE
int ucitaj_pacijente_iz_datoteke(PacijentList* lista) {
    if (lista == NULL) return 0;

    FILE* fp = fopen("kartoni.txt", "r");
    if (fp == NULL) {
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
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;
}