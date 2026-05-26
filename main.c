#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#include "pacijent.h"

PacijentList lista_pacijenata = { NULL, 0, 0 };                    
DvostrukaPovezanaLista dvostuka_lista = { NULL, NULL, 0 };         
Korisnik trenutni_korisnik = { "", "", "" };                      

int main(void) {
    int izbor;
    char pre[50]; 

    if (!inicijalizuj_listu(&lista_pacijenata)) { 
        fprintf(stderr, "[!] Inicijalizacija liste neuspjesna!\n");
        return EXIT_FAILURE;
    }

    if (!inicijalizuj_povezanu_listu(&dvostuka_lista)) { 
        fprintf(stderr, "[!] Inicijalizacija povezane liste neuspjesna!\n");
        return EXIT_FAILURE;
    }

    printf("Učitavanje podataka...\n");
    ucitaj_pacijente_iz_datoteke_binarno(&lista_pacijenata);
    if (lista_pacijenata.count == 0) {
        ucitaj_pacijente_iz_datoteke_tekstualno(&lista_pacijenata); 
    }
    printf("Sustav je spreman!\n\n");

    do {
        ocisti_ekran();
        prikazi_meni();
        izbor = unos_broja("\n >> Vas izbor: ");

        switch (izbor) {
        case 1: 
            procjena_melanoma(); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 2:
            procjena_pluca(); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 3:
            procjena_prostate(); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 4:
            prikazi_pacijente_funkcija(); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 5:
            prikazi_statistiku(); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 6:
            sortiraj_pacijente_po_riziku(); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 7:
            pretraga_rizik(); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 8:
            prikazi_povezanu_listu(&dvostuka_lista); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 9:
            unos_string(pre, sizeof(pre), "Prezime: ");
            azuriraj_rizik_pacijenta(pre); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 10:
            unos_string(pre, sizeof(pre), "Prezime: ");
            obrisi_pacijenta(pre);
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 11:
            pretraga_prezime(); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 12:
            unos_i_prikaz_dnevnika_boravka(); 
            break;
        case 13:
            izvezi_csv_izvjestaj(&lista_pacijenata); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 14:
            provjeri_alarm_rizik(); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 15:
            napravi_backup(); 
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case 0:
            printf("\nSpremanje podataka...\n");
            oslobodi_listu(&lista_pacijenata); 
            oslobodi_povezanu_listu(&dvostuka_lista);
            printf("Ugodan dan, Dovidenja!\n");
            return EXIT_SUCCESS;
        default:
            printf("Nepoznata opcija!\n");
            printf("\nPritisnite enter za nastavak...");
            getchar();
        }

    } while (1); 

    oslobodi_listu(&lista_pacijenata);
    oslobodi_povezanu_listu(&dvostuka_lista);
    return EXIT_SUCCESS;
}