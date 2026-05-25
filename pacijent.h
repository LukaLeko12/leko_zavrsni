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

// --- KONSTANTE --- (2,5: primitivni tipovi, imenovanje)
#define MAX_STRING 100
#define MAX_PATIENTS 1000
#define INITIAL_CAPACITY 50
#define BUFFER_SIZE 256
#define FILE_BINARNI "kartoni.bin"
#define FILE_CSV "izvjestaj.csv"
#define FILE_BACKUP "backup.bin"

// --- ENUM ZA IZBORNIK --- (4,11: enum za izbornik, typedef)
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
} MenuOpcije; // (4,11: enum)

// --- STRUKTURA ZA PACIJENTA --- (2,3,4,15: int, float, struct, typedef, statička polja)
typedef struct {
    char ime[50];               // (15: statički alocirana polja, ne VLA)
    char prezime[50];           // (5: snake_case)
    int godine;                 // (2: primitivni int)
    float rizik_postotak;       // (3: primitivni float)
    char tip_pregleda[30];
    char dijagnoza[100];
    time_t datum_unosa;         // (3: time_t primitivni)
} Pacijent; // (4: typedef struct)

// --- DINAMIČKA LISTA PACIJENATA --- (12,13,16: pokazivači, strukture, dinamička memorija)
typedef struct {
    Pacijent* pacijenti;        // (12: pokazivač na dinamičku memoriju)
    int count;
    int capacity;
} PacijentList; // (4: typedef)

// --- ČVOR ZA POVEZANU LISTU --- (dopunski: dvostruko povezana lista)
typedef struct Node {
    Pacijent data;
    struct Node* next;          // (12: pokazivač)
    struct Node* prev;          // (12: pokazivač)
} Node; // (4: typedef)

// --- DVOSTRUKO POVEZANA LISTA --- (dopunski)
typedef struct {
    Node* head;
    Node* tail;
    int count;
} DvostrukaPovezanaLista; // (4: typedef)

// --- STRUKTURA ZA DNEVNIK --- (3,4,13: složeni tip, typedef, struktura)
typedef struct {
    int dan;                    // (2: int primitivni)
    char osjecaj[100];          // (15: statički alocirana polja)
    float temperatura;          // (3: float primitivni)
    char napomena[200];         // (15: statički alocirana polja)
} DnevnikBoravka; // (4: typedef)

// --- STRUKTURA ZA KORISNIKA --- (3,4: složeni tip, typedef)
typedef struct {
    char korisnicko_ime[50];
    char lozinka[50];
    char tip[20];               // "doktor", "pacijent"
} Korisnik; // (4: typedef)

// --- MAKRO FUNKCIJE --- (9: makro funkcije za jednostavne operacije)
#define PROVJERA_MEMORIJE(ptr) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "[!] GRESKA: Alokacija memorije neuspjesna!\n"); \
            perror("malloc"); \
            exit(EXIT_FAILURE); \
        } \
    } while(0) // (9,18,22: makro, provjera memorije, perror)

#define PROVJERA_DATOTEKE(fp) \
    do { \
        if ((fp) == NULL) { \
            fprintf(stderr, "[!] GRESKA: Datoteka ne moze biti otvorena!\n"); \
            perror("fopen"); \
        } \
    } while(0) // (22: perror)

#define PROVJERA_NULL(ptr) \
    do { \
        if ((ptr) == NULL) return 0; \
    } while(0) // (9,14: zaštita parametara)

// --- GLOBALNE VARIJABLE --- (6,8: static/extern - SAMO DEKLARACIJE!)
extern PacijentList lista_pacijenata;           // (8: extern - samo deklaracija!)
extern DvostrukaPovezanaLista dvostuka_lista;   // (8: extern - samo deklaracija!)
extern Korisnik trenutni_korisnik;              // (8: extern - korisnik)

// --- FUNKCIJE ZA UPRAVLJANJE LISTOM (lista.c) --- (13: strukture i funkcije)
int inicijalizuj_listu(PacijentList* lista);
int prosliri_listu(PacijentList* lista);
int dodaj_pacijenta_u_listu(PacijentList* lista, const Pacijent* pacijent);
void oslobodi_listu(PacijentList* lista);

// --- FUNKCIJE ZA POVEZANU LISTU --- (dopunski: dvostruko povezana lista)
int inicijalizuj_povezanu_listu(DvostrukaPovezanaLista* lista);
int dodaj_u_povezanu_listu(DvostrukaPovezanaLista* lista, const Pacijent* pacijent);
void prikazi_povezanu_listu(const DvostrukaPovezanaLista* lista);
void oslobodi_povezanu_listu(DvostrukaPovezanaLista* lista);
int obrisi_iz_povezane_liste(DvostrukaPovezanaLista* lista, const char* prezime);
Node* pretraga_u_povezanoj_listi(DvostrukaPovezanaLista* lista, const char* prezime);

// --- FUNKCIJE ZA DATOTEKE (datoteke.c) --- (1,19: CRUD, datoteke)
void spremi_u_datoteku_tekstualno(const Pacijent* p);
int ucitaj_pacijente_iz_datoteke_tekstualno(PacijentList* lista);
void spremi_u_datoteku_binarno(const Pacijent* p);
int ucitaj_pacijente_iz_datoteke_binarno(PacijentList* lista);
int obrisi_datoteku(void);
int preimenuj_datoteku(const char* stara, const char* nova);
int kopiraj_datoteku(const char* izvor, const char* odrediste);
int napravi_backup(void);

// --- FUNKCIJE ZA CSV IZVJEŠTAJ --- (21: kopiraj, export)
void izvezi_csv_izvjestaj(PacijentList* lista);

// --- POMOĆNE FUNKCIJE (ui.c) --- (12,14: pokazivači, zaštita)
void ocisti_ekran(void);
int unos_broja(const char* pitanje);
char unos_dn(const char* pitanje);
float unos_float(const char* pitanje);
void unos_string(char* buffer, int max_size, const char* prompt);

// --- MODULI PREGLEDA (pacijent.c) --- (13: funkcije i strukture)
void procjena_melanoma(void);
void procjena_pluca(void);
void procjena_prostate(void);

// --- PRIKAZI PODATKE (ui.c) --- (1: READ operacija)
void prikazi_pacijente_funkcija(void);
void prikazi_statistiku(void);
void prikazi_meni(void);

// --- SORTIRANJE I PRETRAŽIVANJE --- (23,26: qsort, pokazivač na funkciju)
void sortiraj_pacijente_po_riziku(void);
void pretraga_rizik(void);
void pretraga_prezime(void);

// --- UPDATE/DELETE --- (1: UPDATE, DELETE iz CRUID)
void azuriraj_rizik_pacijenta(const char* prezime);
void obrisi_pacijenta(const char* prezime);

// --- DNEVNIK BORAVKA --- (16,17,18: dinamička memorija)
void unos_i_prikaz_dnevnika_boravka(void);

// --- ALARM RIZIK --- (22: upravljanje greškama, notifikacije)
void provjeri_alarm_rizik(void);

// --- AUTENTIFIKACIJA --- (9: makro/inline logika)
int login_korisnik(void);

#endif