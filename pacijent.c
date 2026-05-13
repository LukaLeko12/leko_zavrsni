#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#include "pacijent.h"

extern PacijentList lista_pacijenata;

extern void spremi_u_datoteku(const Pacijent* p);
extern void ocisti_ekran(void);
extern void unos_string(char* buffer, int max_size, const char* prompt);
extern int unos_broja(const char* pitanje);
extern char unos_dn(const char* pitanje);
extern int dodaj_pacijenta_u_listu(PacijentList* lista, const Pacijent* pacijent);

// ANALIZA MELANOMA
void procjena_melanoma(void) {
    Pacijent p;
    int b = 0;

    ocisti_ekran();
    printf(">>> ANALIZA OBLIKA MELANOMA <<<\n\n");

    unos_string(p.ime, sizeof(p.ime), "Ime: ");
    unos_string(p.prezime, sizeof(p.prezime), "Prezime: ");
    p.godine = unos_broja("Godine: ");
    strcpy(p.tip_pregleda, "Melanom");

    printf("\n---- MELANOM (ABCD KRITERIJ) ----\n");
    if (unos_dn("Asimetrija? (da/ne): ") == 'd') b += 25;
    if (unos_dn("Rubovi? (da/ne): ") == 'd') b += 25;
    if (unos_dn("Boja na kozi? (da/ne): ") == 'd') b += 25;
    if (unos_dn("Promjer > 6mm? (da/ne): ") == 'd') b += 25;

    p.rizik_postotak = (float)b;

    if (b >= 75) strcpy(p.dijagnoza, "VISOK RIZIK");
    else strcpy(p.dijagnoza, "NIZAK/SREDNJI");

    if (dodaj_pacijenta_u_listu(&lista_pacijenata, &p)) {
        spremi_u_datoteku(&p);
        printf("\n[+] Pacijent dodan!\n");
        printf("    Rizik: %.0f%%\n", p.rizik_postotak);
        printf("    Dijagnoza: %s\n", p.dijagnoza);
    }
    else {
        printf("[!] Greska pri dodavanju!\n");
    }
}

// ANALIZA PLUCA
void procjena_pluca(void) {
    Pacijent p;
    int b = 0;

    ocisti_ekran();
    printf(">>> ANALIZA PLUCA <<<\n\n");

    unos_string(p.ime, sizeof(p.ime), "Ime: ");
    unos_string(p.prezime, sizeof(p.prezime), "Prezime: ");
    p.godine = unos_broja("Godine: ");
    strcpy(p.tip_pregleda, "Pluca");

    printf("\n---- RIZIK FAKTORI ----\n");
    if (unos_dn("Pusac? (da/ne): ") == 'd') b += 40;
    if (unos_dn("Kasalj? (da/ne): ") == 'd') b += 40;
    if (unos_dn("Bol u prsima? (da/ne): ") == 'd') b += 20;

    p.rizik_postotak = (float)b;

    if (b >= 60) strcpy(p.dijagnoza, "KRITICNO");
    else strcpy(p.dijagnoza, "PRATITI");

    if (dodaj_pacijenta_u_listu(&lista_pacijenata, &p)) {
        spremi_u_datoteku(&p);
        printf("\n[+] Pacijent dodan!\n");
        printf("    Rizik: %.0f%%\n", p.rizik_postotak);
        printf("    Dijagnoza: %s\n", p.dijagnoza);
    }
    else {
        printf("[!] Greska pri dodavanju!\n");
    }
}

// ANALIZA PROSTATE
void procjena_prostate(void) {
    Pacijent p;
    int b = 0;

    ocisti_ekran();
    printf(">>> ANALIZA PROSTATE <<<\n\n");

    unos_string(p.ime, sizeof(p.ime), "Ime: ");
    unos_string(p.prezime, sizeof(p.prezime), "Prezime: ");
    p.godine = unos_broja("Godine: ");
    strcpy(p.tip_pregleda, "Prostata");

    printf("\n---- SIMPTOMI ----\n");
    if (unos_dn("Problemi s mokrenjem? (da/ne): ") == 'd') b += 50;
    if (unos_dn("Bolovi? (da/ne): ") == 'd') b += 50;

    p.rizik_postotak = (float)b;

    if (b >= 50) strcpy(p.dijagnoza, "DODATNE PRETRAGE");
    else strcpy(p.dijagnoza, "UREDAN NALAZ");

    if (dodaj_pacijenta_u_listu(&lista_pacijenata, &p)) {
        spremi_u_datoteku(&p);
        printf("\n[+] Pacijent dodan!\n");
        printf("    Rizik: %.0f%%\n", p.rizik_postotak);
        printf("    Dijagnoza: %s\n", p.dijagnoza);
    }
    else {
        printf("[!] Greska pri dodavanju!\n");
    }
}