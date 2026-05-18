// pacijent.c -- MODULI ZA UNOS PACIJENATA
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#include "pacijent.h"

// ✅ VISE NEMA GLOBALNIH VARIJABLI OVDJE!

// --- MODULI PREGLEDA --- (13: strukture i funkcije)

// MELANOM - (1: CREATE)
void procjena_melanoma(void) {
    Pacijent p;
    int b = 0;
    ocisti_ekran();
    strcpy(p.tip_pregleda, "Melanom");
    printf(">>> ANALIZA MELANOMA PO KRITERIJU <<<\n\n");
    unos_string(p.ime, sizeof(p.ime), "Ime: ");
    unos_string(p.prezime, sizeof(p.prezime), "Prezime: ");
    p.godine = unos_broja("godine: ");

    if (unos_dn("- Asimetrija? (da/ne): ") == 'd') b += 25;
    if (unos_dn("- Rubovi? (da/ne): ") == 'd') b += 25;
    if (unos_dn("- Boja? (da/ne): ") == 'd') b += 25;
    if (unos_dn("- Promjer > 6mm? (da/ne): ") == 'd') b += 25;

    p.rizik_postotak = (float)b;
    if (b >= 75) strcpy(p.dijagnoza, "VISOK RIZIK");
    else strcpy(p.dijagnoza, "NIZAK/SREDNJI");

    spremi_u_datoteku(&p);
    dodaj_pacijenta_u_listu(&lista_pacijenata, &p);
    dodaj_u_povezanu_listu(&dvostuka_lista, &p);

    printf("\n[+] Pacijent dodan! Rizik: %.0f%%\n", p.rizik_postotak);
}

// PLUĆA
void procjena_pluca(void) {
    Pacijent p;
    int b = 0;
    ocisti_ekran();
    strcpy(p.tip_pregleda, "Pluca");
    printf(">>> ANALIZA PLUCA <<<\n\n");
    unos_string(p.ime, sizeof(p.ime), "Ime: ");
    unos_string(p.prezime, sizeof(p.prezime), "Prezime: ");
    p.godine = unos_broja("godine: ");

    if (unos_dn("- Pusac? (da/ne): ") == 'd') b += 40;
    if (unos_dn("- Kasalj? (da/ne): ") == 'd') b += 40;
    if (unos_dn("- Bol u prsima? (da/ne): ") == 'd') b += 20;

    p.rizik_postotak = (float)b;
    if (b >= 60) strcpy(p.dijagnoza, "KRITICNO");
    else strcpy(p.dijagnoza, "PRATITI");

    spremi_u_datoteku(&p);
    dodaj_pacijenta_u_listu(&lista_pacijenata, &p);
    dodaj_u_povezanu_listu(&dvostuka_lista, &p);

    printf("\n[+] Pacijent dodan! Rizik: %.0f%%\n", p.rizik_postotak);
}

// PROSTATA
void procjena_prostate(void) {
    Pacijent p;
    int b = 0;
    ocisti_ekran();
    strcpy(p.tip_pregleda, "Prostata");
    printf(">>> ANALIZA PROSTATE <<<\n\n");
    unos_string(p.ime, sizeof(p.ime), "Ime: ");
    unos_string(p.prezime, sizeof(p.prezime), "Prezime: ");
    p.godine = unos_broja("godine: ");

    if (unos_dn("- Probleme s mokrenjem? (da/ne): ") == 'd') b += 50;
    if (unos_dn("- Bolovi? (da/ne): ") == 'd') b += 50;

    p.rizik_postotak = (float)b;
    if (b >= 50) strcpy(p.dijagnoza, "DODATNE PRETRAGE");
    else strcpy(p.dijagnoza, "UREDAN NALAZ");

    spremi_u_datoteku(&p);
    dodaj_pacijenta_u_listu(&lista_pacijenata, &p);
    dodaj_u_povezanu_listu(&dvostuka_lista, &p);

    printf("\n[+] Pacijent dodan! Rizik: %.0f%%\n", p.rizik_postotak);
}