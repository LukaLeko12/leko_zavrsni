#ifndef PACIJENT_H
#define PACIJENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

// --- KONSTANTE --- 
#define MAX_STRING 100
#define MAX_PATIENTS 1000
#define INITIAL_CAPACITY 50
#define BUFFER_SIZE 256
#define FILE_BINARNI "kartoni.bin"
#define FILE_CSV "izvjestaj.csv"
#define FILE_BACKUP "backup.bin"

// --- ENUM ZA IZBORNIK ---
typedef enum {
    MELANOM = 1,
    PLUCA = 2,
    PROSTATA = 3,
    PRIKAZI = 4,
    STATISTIKA = 5,
    SORTIRAJ_RIZIK = 6,
    PRETRAGA_RIZIK = 7,
    PRIKAZI_POVEZANU = 8,
    AZURIRANJE = 9,
    BRISANJE = 10,
    PRETRAGA_PREZIME = 11,
    DNEVNIK_BORAVKA = 12,
    IZVJESTAJ_CSV = 13,
    ALARM_RIZIK = 14,
    BACKUP = 15,
    IZLAZ = 0
} MenuOpcije; 

// --- STRUKTURA ZA PACIJENTA --- 
typedef struct {
    char ime[50];              
    char prezime[50];          
    int godine;                
    float rizik_postotak;       
    char tip_pregleda[30];
    char dijagnoza[100];
    time_t datum_unosa;         
} Pacijent; 

// --- DINAMIČKA LISTA PACIJENATA --- 
typedef struct {
    Pacijent* pacijenti;       
    int count;
    int capacity;
} PacijentList; 

// --- ČVOR ZA POVEZANU LISTU --- 
typedef struct Node {
    Pacijent data;
    struct Node* next;         
    struct Node* prev;         
} Node; // (4: typedef)

// --- DVOSTRUKO POVEZANA LISTA --- 
typedef struct {
    Node* head;
    Node* tail;
    int count;
} DvostrukaPovezanaLista; 

// --- STRUKTURA ZA DNEVNIK --- 
typedef struct {
    int dan;                    
    char osjecaj[100];         
    float temperatura;          
    char napomena[200];        
} DnevnikBoravka; 

// --- STRUKTURA ZA KORISNIKA --- 
typedef struct {
    char korisnicko_ime[50];
    char lozinka[50];
    char tip[20];              
} Korisnik; 

// --- MAKRO FUNKCIJE --- 
#define PROVJERA_MEMORIJE(ptr) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "[!] GRESKA: Alokacija memorije neuspjesna!\n"); \
            perror("malloc"); \
            exit(EXIT_FAILURE); \
        } \
    } while(0) 

#define PROVJERA_DATOTEKE(fp) \
    do { \
        if ((fp) == NULL) { \
            fprintf(stderr, "[!] GRESKA: Datoteka ne moze biti otvorena!\n"); \
            perror("fopen"); \
        } \
    } while(0) 

#define PROVJERA_NULL(ptr) \
    do { \
        if ((ptr) == NULL) return 0; \
    } while(0) 

// --- GLOBALNE VARIJABLE --- 
extern PacijentList lista_pacijenata;           
extern DvostrukaPovezanaLista dvostuka_lista;  
extern Korisnik trenutni_korisnik;              

// --- FUNKCIJE ZA UPRAVLJANJE LISTOM - lista.c
int inicijalizuj_listu(PacijentList* lista);
int prosliri_listu(PacijentList* lista);
int dodaj_pacijenta_u_listu(PacijentList* lista, const Pacijent* pacijent);
void oslobodi_listu(PacijentList* lista);

// --- FUNKCIJE ZA POVEZANU LISTU ---  
int inicijalizuj_povezanu_listu(DvostrukaPovezanaLista* lista);
int dodaj_u_povezanu_listu(DvostrukaPovezanaLista* lista, const Pacijent* pacijent);
void prikazi_povezanu_listu(const DvostrukaPovezanaLista* lista);
void oslobodi_povezanu_listu(DvostrukaPovezanaLista* lista);
int obrisi_iz_povezane_liste(DvostrukaPovezanaLista* lista, const char* prezime);
Node* pretraga_u_povezanoj_listi(DvostrukaPovezanaLista* lista, const char* prezime);

// --- FUNKCIJE ZA DATOTEKE - datoteke.c
void spremi_u_datoteku_tekstualno(const Pacijent* p);
int ucitaj_pacijente_iz_datoteke_tekstualno(PacijentList* lista);
void spremi_u_datoteku_binarno(const Pacijent* p);
int ucitaj_pacijente_iz_datoteke_binarno(PacijentList* lista);
int obrisi_datoteku(void);
int preimenuj_datoteku(const char* stara, const char* nova);
int kopiraj_datoteku(const char* izvor, const char* odrediste);
int napravi_backup(void);

// --- FUNKCIJE ZA CSV IZVJEŠTAJ --- 
void izvezi_csv_izvjestaj(PacijentList* lista);

// --- POMOĆNE FUNKCIJE - ui.c --- 
void ocisti_ekran(void);
int unos_broja(const char* pitanje);
char unos_dn(const char* pitanje);
float unos_float(const char* pitanje);
void unos_string(char* buffer, int max_size, const char* prompt);

// --- MODULI PREGLEDA - pacijent.c --- 
void procjena_melanoma(void);
void procjena_pluca(void);
void procjena_prostate(void);

// --- PRIKAZI PODATKE - ui.c --- 
void prikazi_pacijente_funkcija(void);
void prikazi_statistiku(void);
void prikazi_meni(void);

// --- SORTIRANJE I PRETRAŽIVANJE --- 
void sortiraj_pacijente_po_riziku(void);
void pretraga_rizik(void);
void pretraga_prezime(void);

// --- UPDATE/DELETE ---
void azuriraj_rizik_pacijenta(const char* prezime);
void obrisi_pacijenta(const char* prezime);

// --- DNEVNIK BORAVKA ---
void unos_i_prikaz_dnevnika_boravka(void);

// --- ALARM RIZIK ---
void provjeri_alarm_rizik(void);

// --- AUTENTIFIKACIJA ---
int login_korisnik(void);

#endif
