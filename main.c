#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#include "pacijent.h"

// GLOBALNA LISTA
PacijentList lista_pacijenata = { NULL, 0, 0 };

int main(void) {
    int izbor;

    // Inicijalizacija liste
    if (!inicijalizuj_listu(&lista_pacijenata)) {
        fprintf(stderr, "[!] Inicijalizacija neuspjesna!\n");
        return EXIT_FAILURE;
    }

    // Učitaj podatke iz datoteke ako postoji
    ucitaj_pacijente_iz_datoteke(&lista_pacijenata);

    do {
        ocisti_ekran();
        prikazi_meni();
        izbor = unos_broja("\n >> Vas izbor: ");

        switch ((MenuOpcije)izbor) {
        case MELANOM:
            procjena_melanoma();
            printf("\nPritisnite enter kako bi ste nastavili...");
            getchar();
            break;
        case PLUCA:
            procjena_pluca();
            printf("\nPritisnite enter kako bi ste nastavili...");
            getchar();
            break;
        case PROSTATA:
            procjena_prostate();
            printf("\nPritisnite enter kako bi ste nastavili...");
            getchar();
            break;
        case PRIKAZI:
            prikazi_sve_pacijente();
            printf("\nPritisnite enter kako bi ste nastavili...");
            getchar();
            break;
        case STATISTIKA:
            prikazi_statistiku();
            printf("\nPritisnite enter kako bi ste nastavili...");
            getchar();
            break;
        case IZLAZ:
            printf("\nUgodan dan, Dovidenja!\n");
            oslobodi_listu(&lista_pacijenata);
            return EXIT_SUCCESS;
        default:
            printf("Nepoznata opcija!\n");
            printf("Pritisnite enter kako bi ste nastavili...");
            getchar();
        }

    } while (1);

    oslobodi_listu(&lista_pacijenata);
    return EXIT_SUCCESS;

    
}