#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#include "pacijent.h"

extern PacijentList lista_pacijenata;

// --- OČISTI EKRAN ---
void ocisti_ekran(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// --- UNOS BROJA ---
int unos_broja(const char* pitanje) {
    int broj;
    printf("%s", pitanje);
    scanf("%d", &broj);
    getchar();
    return broj;
}

// --- UNOS DA/NE ---
char unos_dn(const char* pitanje) {
    char izbor;
    printf("%s", pitanje);
    scanf(" %c", &izbor);
    getchar();
    return izbor;
}

// --- UNOS STRING-A ---
void unos_string(char* buffer, int max_size, const char* prompt) {
    if (buffer == NULL) return;
    printf("%s", prompt);
    if (fgets(buffer, max_size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

// --- PRIKAZI SVE PACIJENTE ---
void prikazi_sve_pacijente(void) {
    ocisti_ekran();
    printf("=== PRIKAZ SVIH PACIJENATA ===\n\n");
    printf("%-15s %-15s %-8s %-12s %-10s %-20s\n",
        "IME", "PREZIME", "GODINE", "TIP", "RIZIK", "DIJAGNOZA");
    printf("--------------------------------------------------------------------------\n");

    if (lista_pacijenata.count == 0) {
        printf("Nema pacijenata!\n");
    }
    else {
        for (int i = 0; i < lista_pacijenata.count; i++) {
            printf("%-15s %-15s %-8d %-12s %-10.0f%% %-20s\n",
                lista_pacijenata.pacijenti[i].ime,
                lista_pacijenata.pacijenti[i].prezime,
                lista_pacijenata.pacijenti[i].godine,
                lista_pacijenata.pacijenti[i].tip_pregleda,
                lista_pacijenata.pacijenti[i].rizik_postotak,
                lista_pacijenata.pacijenti[i].dijagnoza);
        }
    }

    printf("--------------------------------------------------------------------------\n");
    printf("Ukupno: %d pacijenata | Kapacitet: %d\n",
        lista_pacijenata.count, lista_pacijenata.capacity);
}

// --- STATISTIKA ---
void prikazi_statistiku(void) {
    if (lista_pacijenata.count == 0) {
        ocisti_ekran();
        printf("[!] Nema podataka za statistiku!\n");
        return;
    }

    int visok_rizik = 0;
    int srednji_rizik = 0;
    float prosjecan_rizik = 0;

    for (int i = 0; i < lista_pacijenata.count; i++) {
        if (lista_pacijenata.pacijenti[i].rizik_postotak >= 75) {
            visok_rizik++;
        }
        else if (lista_pacijenata.pacijenti[i].rizik_postotak >= 40) {
            srednji_rizik++;
        }
        prosjecan_rizik += lista_pacijenata.pacijenti[i].rizik_postotak;
    }

    ocisti_ekran();
    printf("\n========== STATISTIKA ==========\n\n");
    printf("Ukupno pacijenata: %d\n\n", lista_pacijenata.count);
    printf("Raspodjela po riziku:\n");
    printf("  - Visok rizik (>=75%%):    %d pacijenata (%.1f%%)\n",
        visok_rizik, (float)visok_rizik / lista_pacijenata.count * 100);
    printf("  - Srednji rizik (40-74%%): %d pacijenata (%.1f%%)\n",
        srednji_rizik, (float)srednji_rizik / lista_pacijenata.count * 100);
    printf("  - Nizak rizik (<40%%):     %d pacijenata (%.1f%%)\n",
        lista_pacijenata.count - visok_rizik - srednji_rizik,
        (float)(lista_pacijenata.count - visok_rizik - srednji_rizik) / lista_pacijenata.count * 100);
    printf("\nProsjekurni rizik: %.2f%%\n", prosjecan_rizik / lista_pacijenata.count);
}

// --- PRIKAZI MENI ---
void prikazi_meni(void) {
    printf("\n-----------------------------------------------------\n");
    printf("     SUSTAV ZA PROCJENU ONKOLOSKIH BOLESTI\n");
    printf("-----------------------------------------------------\n");
    printf(" [1] Dodaj - Melanom\n");
    printf(" [2] Dodaj - Pluca\n");
    printf(" [3] Dodaj - Prostata\n");
    printf(" [4] Prikazi sve pacijente\n");
    printf(" [5] Statistika\n");
    printf(" [0] Izlaz\n");
    printf("-----------------------------------------------------\n");
}