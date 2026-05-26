#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#include "pacijent.h"

// --- MODULI --- 

void procjena_melanoma(void) {
    Pacijent p; 
    int b = 0;
    ocisti_ekran();
    strcpy(p.tip_pregleda, "Melanom");
    printf(">>> ANALIZA MELANOMA (ABCD KRITERIJ) <<<\n\n");
    unos_string(p.ime, sizeof(p.ime), "Ime: "); 
    unos_string(p.prezime, sizeof(p.prezime), "Prezime: ");
    p.godine = unos_broja("godine: "); 
    p.datum_unosa = time(NULL);

    if (unos_dn("- Asimetrija? (da/ne): ") == 'd') b += 25;
    if (unos_dn("- Rubovi? (da/ne): ") == 'd') b += 25;
    if (unos_dn("- Boja? (da/ne): ") == 'd') b += 25;
    if (unos_dn("- Promjer > 6mm? (da/ne): ") == 'd') b += 25;

    p.rizik_postotak = (float)b; 
    if (b >= 75) strcpy(p.dijagnoza, "VISOK RIZIK - HITNO!");
    else strcpy(p.dijagnoza, "NIZAK/SREDNJI");

    spremi_u_datoteku_binarno(&p); 
    spremi_u_datoteku_tekstualno(&p); 
    dodaj_pacijenta_u_listu(&lista_pacijenata, &p); 
    dodaj_u_povezanu_listu(&dvostuka_lista, &p); 

    printf("\n[+] Pacijent dodan! Rizik: %.0f%%\n", p.rizik_postotak);

   
    if (p.rizik_postotak >= 75) {
        printf("\nUPOZORENJE: VISOK RIZIK - HITNA PRETRAGA PREPORUCENA!\n");
    }
}

void procjena_pluca(void) {
    Pacijent p; 
    int b = 0;
    ocisti_ekran();
    strcpy(p.tip_pregleda, "Pluca");
    printf(">>> ANALIZA PLUCA <<<\n\n");
    unos_string(p.ime, sizeof(p.ime), "Ime: ");
    unos_string(p.prezime, sizeof(p.prezime), "Prezime: ");
    p.godine = unos_broja("godine: ");
    p.datum_unosa = time(NULL);

    if (unos_dn("- Pusac? (da/ne): ") == 'd') b += 40;
    if (unos_dn("- Kasalj? (da/ne): ") == 'd') b += 40;
    if (unos_dn("- Bol u prsima? (da/ne): ") == 'd') b += 20;

    p.rizik_postotak = (float)b;
    if (b >= 60) strcpy(p.dijagnoza, "KRITICNO - HITNO!");
    else strcpy(p.dijagnoza, "PRATITI");

    spremi_u_datoteku_binarno(&p); 
    spremi_u_datoteku_tekstualno(&p);
    dodaj_pacijenta_u_listu(&lista_pacijenata, &p); 
    dodaj_u_povezanu_listu(&dvostuka_lista, &p);

    printf("\n[+] Pacijent dodan! Rizik: %.0f%%\n", p.rizik_postotak);

    if (p.rizik_postotak >= 60) {
        printf("\nUPOZORENJE: KRITICNO STANJE - ODMAH OBAVIJESTITI LIJECNIKA!\n");
    }
}

void procjena_prostate(void) {
    Pacijent p; 
    int b = 0;
    ocisti_ekran();
    strcpy(p.tip_pregleda, "Prostata");
    printf(">>> ANALIZA PROSTATE <<<\n\n");
    unos_string(p.ime, sizeof(p.ime), "Ime: ");
    unos_string(p.prezime, sizeof(p.prezime), "Prezime: ");
    p.godine = unos_broja("godine: ");
    p.datum_unosa = time(NULL);

    if (unos_dn("- Probleme s mokrenjem? (da/ne): ") == 'd') b += 50;
    if (unos_dn("- Bolovi? (da/ne): ") == 'd') b += 50;

    p.rizik_postotak = (float)b;
    if (b >= 50) strcpy(p.dijagnoza, "DODATNE PRETRAGE POTREBNE");
    else strcpy(p.dijagnoza, "UREDAN NALAZ");

    spremi_u_datoteku_binarno(&p); 
    spremi_u_datoteku_tekstualno(&p);
    dodaj_pacijenta_u_listu(&lista_pacijenata, &p); 
    dodaj_u_povezanu_listu(&dvostuka_lista, &p);

    printf("\n[+] Pacijent dodan! Rizik: %.0f%%\n", p.rizik_postotak);

    if (p.rizik_postotak >= 50) {
        printf("\nUPOZORENJE: PREPORUCITE DODATNE PRETRAGE!\n");
    }
}

// --- UPDATE ---
void azuriraj_rizik_pacijenta(const char* prezime) {
    if (prezime == NULL) return; 

    for (int i = 0; i < lista_pacijenata.count; i++) {
        if (strcmp(lista_pacijenata.pacijenti[i].prezime, prezime) == 0) {
            printf("Stari rizik: %.0f%%\n", lista_pacijenata.pacijenti[i].rizik_postotak);
            lista_pacijenata.pacijenti[i].rizik_postotak = unos_float("Novi rizik (%%) - ");
            printf("[+] Azuriran!\n");
            return;
        }
    }
    printf("[!] Nema pacijenta s tim prezimenom.\n");
}

// --- DELETE --- 
void obrisi_pacijenta(const char* prezime) {
    if (prezime == NULL) return; 

    for (int i = 0; i < lista_pacijenata.count; i++) {
        if (strcmp(lista_pacijenata.pacijenti[i].prezime, prezime) == 0) {
            for (int j = i; j < lista_pacijenata.count - 1; j++) {
                lista_pacijenata.pacijenti[j] = lista_pacijenata.pacijenti[j + 1];
            }
            lista_pacijenata.count--;
            obrisi_iz_povezane_liste(&dvostuka_lista, prezime);
            printf("[+] Pacijent obrisan!\n");
            return;
        }
    }
    printf("[!] Nema pacijenta za brisanje!\n");
}