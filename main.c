// main.c -- GLAVNA DATOTEKA
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#include "pacijent.h"

// ✅ SAMO OVDJE DEFINIRAJ GLOBALNE VARIJABLE! (6,8: static/extern - DEFINICIJA)
PacijentList lista_pacijenata = { NULL, 0, 0 };                    // (6,8: DEFINICIJA!)
DvostrukaPovezanaLista dvostuka_lista = { NULL, NULL, 0 };         // (6,8: DEFINICIJA!)
Korisnik trenutni_korisnik = { "", "", "" };                       // (6,8: korisnik - DEFINICIJA!)

int main(void) {
    int izbor;
    char pre[50]; // (15: statički alocirana polja, ne VLA)

    // --- INICIJALIZACIJA --- (16,17: dinamička memorija)
    if (!inicijalizuj_listu(&lista_pacijenata)) { // (13: funkcije)
        fprintf(stderr, "[!] Inicijalizacija liste neuspjesna!\n");
        return EXIT_FAILURE;
    }

    if (!inicijalizuj_povezanu_listu(&dvostuka_lista)) { // (dopunski)
        fprintf(stderr, "[!] Inicijalizacija povezane liste neuspjesna!\n");
        return EXIT_FAILURE;
    }

    // --- UČITAVANJE IZ DATOTEKE (BINARNE I TEKSTUALNE) --- (1: READ)
    printf("⏳ Učitavanje podataka...\n");
    ucitaj_pacijente_iz_datoteke_binarno(&lista_pacijenata); // (1: READ - binarna)
    if (lista_pacijenata.count == 0) {
        ucitaj_pacijente_iz_datoteke_tekstualno(&lista_pacijenata); // (1: READ - tekstualna, fallback)
    }
    printf("Sustav spreman!\n\n");

    // --- MAIN LOOP --- (10,11: izbornik, enum)
    do {
        ocisti_ekran();
        prikazi_meni();
        izbor = unos_broja("\n >> Vas izbor: ");

        switch (izbor) {
        case 1: // (11: enum - ali brojevi za lakšu ekspanziju)
            procjena_melanoma(); // (1: CREATE)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 2:
            procjena_pluca(); // (1: CREATE)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 3:
            procjena_prostate(); // (1: CREATE)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 4:
            prikazi_pacijente_funkcija(); // (1: READ)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 5:
            prikazi_statistiku(); // (1: READ)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 6:
            sortiraj_pacijente_po_riziku(); // (23,26: qsort)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 7:
            pretraga_rizik(); // (24: pretraživanje)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 8:
            prikazi_povezanu_listu(&dvostuka_lista); // (dopunski)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 9:
            unos_string(pre, sizeof(pre), "Prezime: "); // (14: zaštita)
            azuriraj_rizik_pacijenta(pre); // (1: UPDATE)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 10:
            unos_string(pre, sizeof(pre), "Prezime: ");
            obrisi_pacijenta(pre); // (1: DELETE)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 11:
            pretraga_prezime(); // (24: pretraživanje u povezanoj listi)
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 12:
            unos_i_prikaz_dnevnika_boravka(); // DNEVNIK BORAVKA
            break;
        case 13:
            izvezi_csv_izvjestaj(&lista_pacijenata); // IZVJEŠTAJ CSV
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 14:
            provjeri_alarm_rizik(); // ALARM RIZIK
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 15:
            napravi_backup(); // BACKUP
            printf("\nEnter za nastavak...");
            getchar();
            break;
        case 0:
            printf("\nSpremanje podataka...\n");
            oslobodi_listu(&lista_pacijenata); // (18: oslobađanje)
            oslobodi_povezanu_listu(&dvostuka_lista);
            printf("Dovidenja!\n");
            return EXIT_SUCCESS;
        default:
            printf("Nepoznata opcija!\n");
            printf("\nEnter za nastavak...");
            getchar();
        }

    } while (1); // (10: izbornik loop)

    oslobodi_listu(&lista_pacijenata);
    oslobodi_povezanu_listu(&dvostuka_lista);
    return EXIT_SUCCESS;
}