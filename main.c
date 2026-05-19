#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#include "pacijent.h"

// (6,8 static, extern)
PacijentList lista_pacijenata = { NULL, 0, 0 };                    // (6,8)
DvostrukaPovezanaLista dvostuka_lista = { NULL, NULL, 0 };         // (6,8)

int main(void) {
    int izbor;

    // ---- INICIJALIZACIJA ---- (16,17 dinamička memorija)
    if (!inicijalizuj_listu(&lista_pacijenata)) {
        fprintf(stderr, "[!] Inicijalizacija liste neuspjesna!\n");
        return EXIT_FAILURE;
    }

    if (!inicijalizuj_povezanu_listu(&dvostuka_lista)) {
        fprintf(stderr, "[!] Inicijalizacija povezane liste neuspjesna!\n");
        return EXIT_FAILURE;
    }

    // ---- UČITAVANJE IZ DATOTEKE ---- (1 READ)
    ucitaj_pacijente_iz_datoteke(&lista_pacijenata);

    // ---- MAIN LOOP ---- (10,11 izbornik, enum)
    do {
        ocisti_ekran();
        prikazi_meni();
        izbor = unos_broja("\n >> Vas izbor: ");

        switch ((MenuOpcije)izbor) {
        case MELANOM:
            procjena_melanoma();
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case PLUCA:
            procjena_pluca();
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case PROSTATA:
            procjena_prostate();
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case PRIKAZI:
            prikazi_pacijente_funkcija();
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case STATISTIKA:
            prikazi_statistiku();
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case SORTIRAJ_RIZIK:
            sortiraj_pacijente_po_riziku();
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case PRETRAGA_RIZIK:
            pretraga_rizik();
            printf("\nPritisnite enter za nastavak...");
            getchar();
            break;
        case IZLAZ:
            printf("\nUgodan dan, Dovidenja!\n");
            oslobodi_listu(&lista_pacijenata);                // (18 oslobađanje memorije)
            oslobodi_povezanu_listu(&dvostuka_lista);
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