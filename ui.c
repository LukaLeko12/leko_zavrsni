// ui.c -- KORISNIČKI UI
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#include "pacijent.h"

// --- ČISTI EKRAN --- (7: organizacija)
void ocisti_ekran(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// --- UNOS BROJA --- (12,14: pokazivači, zaštita)
int unos_broja(const char* pitanje) {
    int broj;
    printf("%s", pitanje);
    if (scanf("%d", &broj) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    getchar();
    return broj;
}

// --- UNOS DA/NE --- (12,14)
char unos_dn(const char* pitanje) {
    char izbor;
    printf("%s", pitanje);
    scanf(" %c", &izbor);
    getchar();
    return izbor;
}

// --- UNOS STRINGA --- (12,14: pokazivač, zaštita)
void unos_string(char* buffer, int max_size, const char* prompt) {
    if (buffer == NULL) return; // (14: zaštita)
    printf("%s", prompt);
    if (fgets(buffer, max_size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

// --- PRIKAZI SVE PACIJENTE --- (1: READ)
void prikazi_pacijente_funkcija(void) {
    ocisti_ekran();
    printf("---- PRIKAZ SVIH PACIJENATA ----\n");
    printf("%-12s %-12s %-4s %-15s %-8s %-30s\n",
        "IME", "PREZIME", "GOD", "TIP", "RIZIK", "DIJAGNOZA");
    printf("---------------------------------------------------------------\n");

    if (lista_pacijenata.count == 0) {
        printf("Nema pacijenata!\n");
    }
    else {
        for (int i = 0; i < lista_pacijenata.count; i++) {
            printf("%-12s %-12s %-4d %-15s %-7.2f%% %-30s\n",
                lista_pacijenata.pacijenti[i].ime,
                lista_pacijenata.pacijenti[i].prezime,
                lista_pacijenata.pacijenti[i].godine,
                lista_pacijenata.pacijenti[i].tip_pregleda,
                lista_pacijenata.pacijenti[i].rizik_postotak,
                lista_pacijenata.pacijenti[i].dijagnoza);
        }
    }

    printf("---------------------------------------------------------------\n");
    printf("Ukupno: %d pacijenata\n", lista_pacijenata.count);
}

// --- STATISTIKA --- (1: READ)
void prikazi_statistiku(void) {
    if (lista_pacijenata.count == 0) {
        printf(" [!] Nema podataka za statistiku!\n");
        return;
    }

    int visok_rizik = 0;
    float prosjecan_rizik = 0;

    for (int i = 0; i < lista_pacijenata.count; i++) {
        if (lista_pacijenata.pacijenti[i].rizik_postotak >= 75) {
            visok_rizik++;
        }
        prosjecan_rizik += lista_pacijenata.pacijenti[i].rizik_postotak;
    }

    ocisti_ekran();
    printf("\n---------- STATISTIKA - PACIJENATA ----------\n");
    printf("Ukupno pacijenata: %d\n", lista_pacijenata.count);
    printf("Visok rizik (>=75%%): %d (%.1f%%)\n", visok_rizik,
        (float)visok_rizik / lista_pacijenata.count * 100);
    printf("Nizak rizik: %d (%.1f%%)\n", lista_pacijenata.count - visok_rizik,
        (float)(lista_pacijenata.count - visok_rizik) / lista_pacijenata.count * 100);
    printf("Prosjecan rizik: %.2f%%\n", prosjecan_rizik / lista_pacijenata.count);
}

// --- PRETRAGA PO RIZIKU --- (1: READ)
void pretraga_rizik(void) {
    ocisti_ekran();
    int min_rizik = unos_broja("Unesite minimalni rizik [%]: ");

    if (lista_pacijenata.count == 0) {
        printf(" [!] Nema podataka!\n");
        return;
    }

    printf("\n--- Pacijenti sa rizikom >= %d%% ---\n", min_rizik);
    printf("%-12s %-12s %-4s %-15s %-8s %-30s\n",
        "IME", "PREZIME", "GOD", "TIP", "RIZIK", "DIJAGNOZA");
    printf("---------------------------------------------------------------\n");

    int pronaden = 0;
    for (int i = 0; i < lista_pacijenata.count; i++) {
        if (lista_pacijenata.pacijenti[i].rizik_postotak >= min_rizik) {
            printf("%-12s %-12s %-4d %-15s %-7.2f%% %-30s\n",
                lista_pacijenata.pacijenti[i].ime,
                lista_pacijenata.pacijenti[i].prezime,
                lista_pacijenata.pacijenti[i].godine,
                lista_pacijenata.pacijenti[i].tip_pregleda,
                lista_pacijenata.pacijenti[i].rizik_postotak,
                lista_pacijenata.pacijenti[i].dijagnoza);
            pronaden++;
        }
    }

    if (pronaden == 0) {
        printf("Nema pacijenata sa tim rizikom!\n");
    }
    else {
        printf("Pronaden: %d pacijenata\n", pronaden);
    }
}

// --- KOMPARATOR ZA SORTIRANJE --- (23,26: qsort, pokazivač na funkciju)
static int compare_pacijenti_po_riziku(const void* a, const void* b) {
    const Pacijent* p1 = (const Pacijent*)a;
    const Pacijent* p2 = (const Pacijent*)b;

    if (p1->rizik_postotak < p2->rizik_postotak) return -1;
    if (p1->rizik_postotak > p2->rizik_postotak) return 1;
    return 0;
}

// --- SORTIRANJE PO RIZIKU --- (23,26: qsort)
void sortiraj_pacijente_po_riziku(void) {
    ocisti_ekran();
    printf("---- SORTIRANJE PO RIZIKU ----\n");

    if (lista_pacijenata.count == 0) {
        printf("Nema pacijenata za sortiranje!\n");
        return;
    }

    qsort(lista_pacijenata.pacijenti, // (23,26: qsort + pokazivač na funkciju)
        lista_pacijenata.count,
        sizeof(Pacijent),
        compare_pacijenti_po_riziku);

    printf("[+] Sortirano!\n");
    printf("%-12s %-12s %-4s %-15s %-8s %-30s\n",
        "IME", "PREZIME", "GOD", "TIP", "RIZIK", "DIJAGNOZA");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < lista_pacijenata.count; i++) {
        printf("%-12s %-12s %-4d %-15s %-7.2f%% %-30s\n",
            lista_pacijenata.pacijenti[i].ime,
            lista_pacijenata.pacijenti[i].prezime,
            lista_pacijenata.pacijenti[i].godine,
            lista_pacijenata.pacijenti[i].tip_pregleda,
            lista_pacijenata.pacijenti[i].rizik_postotak,
            lista_pacijenata.pacijenti[i].dijagnoza);
    }
}

// --- MENI --- (10,11: izbornik, enum)
void prikazi_meni(void) {
    printf("---------------------------------------------------\n");
    printf("     SUSTAV ZA PROCJENU ONKOLOSKIH BOLESTI\n");
    printf("---------------------------------------------------\n");
    printf(" [1] Melanom - ABCD kriterij\n"
        " [2] Pluca - Anamneza\n"
        " [3] Prostata - Simptomi\n"
        " [4] Prikazi sve pacijente\n"
        " [5] Statistika\n"
        " [6] Sortiraj po riziku (qsort)\n"
        " [7] Pretraga po riziku\n"
        " [0] Izlaz\n");
    printf("---------------------------------------------------\n");
}