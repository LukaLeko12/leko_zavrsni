#define _CRT_SECURE_NO_WARNINGS
#include "pacijent.h"

void spremi_u_datoteku_tekstualno(const Pacijent* p) {
    if (p == NULL) return;

    FILE* fp = fopen("kartoni.txt", "a"); 
    if (fp == NULL) { 
        printf(" [!] Greska pri otvaranju datoteke!\n");
        perror("fopen"); 
        return;
    }

    fprintf(fp, "%-12s %-12s %-4d %-15s %-7.2f%% %-30s\n",
        p->ime, p->prezime, p->godine, p->tip_pregleda,
        p->rizik_postotak, p->dijagnoza);

    if (ferror(fp)) { 
        printf("[!] Greska pri pisanju u datoteku!\n");
        perror("fprintf");
    }

    fclose(fp); 
}

int ucitaj_pacijente_iz_datoteke_tekstualno(PacijentList* lista) {
    if (lista == NULL) return 0; // (14)

    FILE* fp = fopen("kartoni.txt", "r"); 
    if (fp == NULL) { 
        printf(" Tekstualna datoteka ne postoji ili je prazna.\n");
        return 1;
    }

    Pacijent temp_pacijent;
    while (fscanf(fp, "%49s %49s %d %29s %f %99s",
        temp_pacijent.ime,
        temp_pacijent.prezime,
        &temp_pacijent.godine,
        temp_pacijent.tip_pregleda,
        &temp_pacijent.rizik_postotak,
        temp_pacijent.dijagnoza) == 6) {

        if (!dodaj_pacijenta_u_listu(lista, &temp_pacijent)) {
            fclose(fp); 
            return 0;
        }

        dodaj_u_povezanu_listu(&dvostuka_lista, &temp_pacijent);
    }

    if (feof(fp)) { 
       
    }
    else if (ferror(fp)) { 
        printf("[!] Greska pri čitanju datoteke!\n");
        perror("fscanf");
    }

    fclose(fp);
    return 1;
}

void spremi_u_datoteku_binarno(const Pacijent* p) {
    if (p == NULL) return; 

    FILE* fp = fopen(FILE_BINARNI, "ab"); 
    if (fp == NULL) { 
        printf(" [!] Greska pri otvaranju binarne datoteke!\n");
        perror("fopen");
        return;
    }

    if (fwrite(p, sizeof(Pacijent), 1, fp) != 1) { 
        printf("[!] Greska pri pisanju binarne datoteke!\n");
        perror("fwrite");
    }

    fclose(fp); 
}

int ucitaj_pacijente_iz_datoteke_binarno(PacijentList* lista) {
    if (lista == NULL) return 0; 

    FILE* fp = fopen(FILE_BINARNI, "rb"); 
    if (fp == NULL) { 
        printf(" Binarna datoteka ne postoji ili je prazna.\n");
        return 1;
    }

    Pacijent temp_pacijent;
    while (fread(&temp_pacijent, sizeof(Pacijent), 1, fp) == 1) { 
        if (!dodaj_pacijenta_u_listu(lista, &temp_pacijent)) {
            fclose(fp);
            return 0;
        }

        dodaj_u_povezanu_listu(&dvostuka_lista, &temp_pacijent);
    }

    if (ferror(fp)) { 
        printf("[!] Greska pri čitanju binarne datoteke!\n");
        perror("fread");
    }

    fclose(fp); 
    return 1;
}

int obrisi_datoteku(void) {
    if (remove(FILE_BINARNI) == 0) { 
        printf("[+] Binarna datoteka obrisana!\n");
        return 1;
    }
    else {
        printf("[!] Greska pri brisanju binarne datoteke!\n");
        perror("remove"); 
        return 0;
    }
}

int preimenuj_datoteku(const char* stara, const char* nova) {
    if (stara == NULL || nova == NULL) return 0; 

    if (rename(stara, nova) == 0) { 
        printf("[+] Datoteka preimenovana: %s -> %s\n", stara, nova);
        return 1;
    }
    else {
        printf("[!] Greska pri preimenovanju datoteke!\n");
        perror("rename"); 
        return 0;
    }
}

int kopiraj_datoteku(const char* izvor, const char* odrediste) {
    if (izvor == NULL || odrediste == NULL) return 0; 

    FILE* src = fopen(izvor, "rb"); 
    if (src == NULL) { 
        printf("[!] Greska pri otvaranju izvorne datoteke!\n");
        perror("fopen");
        return 0;
    }

    FILE* dst = fopen(odrediste, "wb"); 
    if (dst == NULL) { 
        printf("[!] Greska pri otvaranju odredisne datoteke!\n");
        perror("fopen");
        fclose(src);
        return 0;
    }

    char buffer[BUFFER_SIZE]; 
    size_t bytes;
    while ((bytes = fread(buffer, 1, BUFFER_SIZE, src)) > 0) { 
        fwrite(buffer, 1, bytes, dst); 
    }

    if (ferror(src) || ferror(dst)) { 
        printf("[!] Greska tijekom kopiranja!\n");
        perror("read/write");
        fclose(src);
        fclose(dst);
        return 0;
    }

    fclose(src); 
    fclose(dst); 
    printf("[+] Datoteka kopirana: %s -> %s\n", izvor, odrediste);
    return 1;
}

int napravi_backup(void) {
    if (kopiraj_datoteku(FILE_BINARNI, FILE_BACKUP)) {
        printf("[+] Backup napravljen!\n");
        return 1;
    }
    return 0;
}

void izvezi_csv_izvjestaj(PacijentList* lista) {
    if (lista == NULL) return; 

    FILE* fp = fopen(FILE_CSV, "w"); 
    if (fp == NULL) { 
        printf("[!] Greska pri pisanju CSV datoteke!\n");
        perror("fopen");
        return;
    }

    fprintf(fp, "Ime,Prezime,Godine,Tip_Pregleda,Rizik_Postotak,Dijagnoza\n");

    for (int i = 0; i < lista->count; i++) {
        fprintf(fp, "%s,%s,%d,%s,%.2f,%s\n",
            lista->pacijenti[i].ime,
            lista->pacijenti[i].prezime,
            lista->pacijenti[i].godine,
            lista->pacijenti[i].tip_pregleda,
            lista->pacijenti[i].rizik_postotak,
            lista->pacijenti[i].dijagnoza);
    }

    fclose(fp);
    printf("[+] CSV izvjestaj napravljen: %s\n", FILE_CSV);

    void azuriraj_tekstualnu_datoteku(void) {
        FILE* fp = fopen("kartoni.txt", "w"); 
        if (!fp) {
            printf("[!] Greska pri azuriranju tekstualne datoteke!\n");
            return;
        }
        for (int i = 0; i < lista_pacijenata.count; i++) {
            fprintf(fp, "%-12s %-12s %-4d %-15s %-7.2f%% %-30s\n",
                lista_pacijenata.pacijenti[i].ime,
                lista_pacijenata.pacijenti[i].prezime,
                lista_pacijenata.pacijenti[i].godine,
                lista_pacijenata.pacijenti[i].tip_pregleda,
                lista_pacijenata.pacijenti[i].rizik_postotak,
                lista_pacijenata.pacijenti[i].dijagnoza
            );
        }
        fclose(fp);
    }
}