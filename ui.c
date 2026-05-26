#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6031)
#include "pacijent.h"

void ocisti_ekran(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int unos_broja(const char* pitanje) {
    if (pitanje == NULL) return 0; 
    int broj;
    printf("%s", pitanje);
    if (scanf("%d", &broj) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    getchar();
    return broj;
}

char unos_dn(const char* pitanje) {
    if (pitanje == NULL) return 'n'; 
    char izbor;
    printf("%s", pitanje);
    scanf(" %c", &izbor);
    getchar();
    return izbor;
}

float unos_float(const char* pitanje) {
    if (pitanje == NULL) return 0.0f; 
    float broj;
    printf("%s", pitanje);
    if (scanf("%f", &broj) != 1) {
        while (getchar() != '\n');
        return 0.0f;
    }
    getchar();
    return broj;
}

void unos_string(char* buffer, int max_size, const char* prompt) {
    if (buffer == NULL || max_size <= 0) return; 
    printf("%s", prompt);
    if (fgets(buffer, max_size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

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

void prikazi_statistiku(void) {
    if (lista_pacijenata.count == 0) {
        printf(" [!] Nema podataka za statistiku!\n");
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
    printf("\n-------- STATISTIKA --------\n");
    printf("Ukupno pacijenata: %d\n\n", lista_pacijenata.count);
    printf("RASPODJELA PO RIZIKU:\n");
    printf("  Visok rizik (>=75%%):    %d pacijenata (%.1f%%)\n", visok_rizik,
        (float)visok_rizik / lista_pacijenata.count * 100);
    printf("  Srednji rizik (40-74%%): %d pacijenata (%.1f%%)\n",
        srednji_rizik, (float)srednji_rizik / lista_pacijenata.count * 100);
    printf("  Nizak rizik (<40%%):     %d pacijenata (%.1f%%)\n",
        lista_pacijenata.count - visok_rizik - srednji_rizik,
        (float)(lista_pacijenata.count - visok_rizik - srednji_rizik) / lista_pacijenata.count * 100);
    printf("\nProsjecni rizik: %.2f%%\n", prosjecan_rizik / lista_pacijenata.count);
}

void pretraga_rizik(void) {
    ocisti_ekran();
    int min_rizik = unos_broja("Unesite minimalni rizik (%): ");

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
        printf("\nPronaden: %d pacijenata\n", pronaden);
    }
}

void pretraga_prezime(void) {
    ocisti_ekran();
    char pre[50]; 
    unos_string(pre, sizeof(pre), "Unesite prezime: ");

    Node* pronaden = pretraga_u_povezanoj_listi(&dvostuka_lista, pre);
    if (pronaden != NULL) {
        printf("\n[+] Pronađen pacijent:\n");
        printf("%-12s %-12s %-4d %-15s %-7.2f%% %-30s\n",
            pronaden->data.ime,
            pronaden->data.prezime,
            pronaden->data.godine,
            pronaden->data.tip_pregleda,
            pronaden->data.rizik_postotak,
            pronaden->data.dijagnoza);
    }
    else {
        printf("[!] Nema pacijenta s tim prezimenom!\n");
    }
}

static int compare_pacijenti_po_riziku(const void* a, const void* b) {
    const Pacijent* p1 = (const Pacijent*)a; 
    const Pacijent* p2 = (const Pacijent*)b;

    if (p1->rizik_postotak < p2->rizik_postotak) return 1;
    if (p1->rizik_postotak > p2->rizik_postotak) return -1;
    return 0;
}

void sortiraj_pacijente_po_riziku(void) {
    ocisti_ekran();
    printf("---- SORTIRANJE PO RIZIKU ----\n");

    if (lista_pacijenata.count == 0) {
        printf("Nema pacijenata za sortiranje!\n");
        return;
    }

    qsort(lista_pacijenata.pacijenti,
        lista_pacijenata.count,
        sizeof(Pacijent),
        compare_pacijenti_po_riziku);

    printf("[+] Sortirano od najveceg do najmanjeg rizika!\n");
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

void unos_i_prikaz_dnevnika_boravka(void) {
    ocisti_ekran();
    int broj_dana = unos_broja("Koliko je pacijent proveo dana u bolnici? "); 

    if (broj_dana <= 0) {
        printf("[!] Broj dana mora biti pozitivan!\n");
        return;
    }

    DnevnikBoravka* dnevnik = (DnevnikBoravka*)malloc(broj_dana * sizeof(DnevnikBoravka)); 
    if (dnevnik == NULL) {
        printf("[!] Greska pri alokaciji memorije!\n");
        perror("malloc");
        return;
    }

    for (int i = 0; i < broj_dana; i++) {
        printf("\n---- DAN %d ----\n", i + 1);
        dnevnik[i].dan = i + 1; 

        unos_string(dnevnik[i].osjecaj, sizeof(dnevnik[i].osjecaj),
            "Kako se osjecate? (Lose/Prihvatljivo/Dobro/Odlicno): ");

        dnevnik[i].temperatura = unos_float("Temperatura (°C): "); 

        unos_string(dnevnik[i].napomena, sizeof(dnevnik[i].napomena),
            "Dodatne napomene: ");
    }

    ocisti_ekran();
    printf("\n---- DNEVNIK BORAVKA U BOLNICI ----\n");
    printf("%-5s %-15s %-12s %-30s\n", "DAN", "OSJECAJ", "TEMP (°C)", "NAPOMENA");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < broj_dana; i++) {
        printf("%-5d %-15s %-12.1f %-30s\n",
            dnevnik[i].dan,
            dnevnik[i].osjecaj,
            dnevnik[i].temperatura,
            dnevnik[i].napomena);
    }

    float prosj_temp = 0.0f; 
    int dobri_dani = 0; 

    for (int i = 0; i < broj_dana; i++) {
        prosj_temp += dnevnik[i].temperatura;

        if (strcmp(dnevnik[i].osjecaj, "Dobro") == 0 ||
            strcmp(dnevnik[i].osjecaj, "Odlicno") == 0) {
            dobri_dani++;
        }
    }

    printf("\n---- STATISTIKA BORAVKA ----\n");
    printf("Broj dana: %d\n", broj_dana);
    printf("Prosjecna temperatura: %.1f °C\n", prosj_temp / broj_dana);
    printf("Dobrih dana: %d (%.1f%%)\n", dobri_dani, (float)dobri_dani / broj_dana * 100);
    printf("Losih dana: %d (%.1f%%)\n", broj_dana - dobri_dani,
        (float)(broj_dana - dobri_dani) / broj_dana * 100);

    free(dnevnik); 
    dnevnik = NULL; 

    printf("\nEnter za nastavak...");
    getchar();
}

void provjeri_alarm_rizik(void) {
    ocisti_ekran();
    printf("---- PROVJERA ALARMA ----\n\n");

    if (lista_pacijenata.count == 0) {
        printf("Nema pacijenata!\n");
        return;
    }

    int broj_alarma = 0;
    printf("Pacijenti sa VISOKIM RIZIKOM (>=75%%):\n");
    printf("%-12s %-12s %-4s %-15s %-8s %-30s\n",
        "IME", "PREZIME", "GOD", "TIP", "RIZIK", "DIJAGNOZA");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < lista_pacijenata.count; i++) {
        if (lista_pacijenata.pacijenti[i].rizik_postotak >= 75) {
            printf("%-12s %-12s %-4d %-15s %-7.2f%% %-30s ALARM!\n",
                lista_pacijenata.pacijenti[i].ime,
                lista_pacijenata.pacijenti[i].prezime,
                lista_pacijenata.pacijenti[i].godine,
                lista_pacijenata.pacijenti[i].tip_pregleda,
                lista_pacijenata.pacijenti[i].rizik_postotak,
                lista_pacijenata.pacijenti[i].dijagnoza);
            broj_alarma++;
        }
    }

    printf("\nUKUPNO ALARMA: %d\n", broj_alarma);
    printf("Preporucuje se HITNA INTERVENCIJA!\n");
}

void prikazi_meni(void) {
    printf("---------------------------------------------------\n");
    printf("     SUSTAV ZA PROCJENU ONKOLOSKIH BOLESTI\n");
    printf("---------------------------------------------------\n");
    printf(" [1]  Melanom\n"
        " [2]  Pluca\n"
        " [3]  Prostata\n"
        " [4]  Prikazi sve pacijente\n"
        " [5]  Statistika\n"
        " [6]  Sortiraj po riziku\n"
        " [7]  Pretraga po riziku\n"
        " [8]  Prikazi povezanu listu\n"
        " [9]  Azuriraj rizik pacijenta\n"
        " [10] Obrisi pacijenta\n"
        " [11] Pretraga po prezimenu\n"
        " [12] Dnevnik boravka pacijenta\n"
        " [13] Izvezi CSV izvjestaj\n"
        " [14] Provjeri alarm\n"
        " [15] Napravi backup\n"
        " [0]  Izlaz iz sustava\n");
    printf("---------------------------------------------------\n");
}