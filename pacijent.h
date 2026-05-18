#ifndef PACIJENT_H
#define PACIJENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#endif

// --- KONSTANTE --- (5: imenovanje)
#define MAX_STRING 100
#define MAX_PATIENTS 1000
#define INITIAL_CAPACITY 50
#define BUFFER_SIZE 256

// --- ENUM ZA IZBORNIK --- (11: enum za izbornik)
typedef enum {
    MELANOM = 1,
    PLUCA = 2,
    PROSTATA = 3,
    PRIKAZI = 4,
    STATISTIKA = 5,
    SORTIRAJ_RIZIK = 6,
    PRETRAGA_RIZIK = 7,
    IZLAZ = 0
} MenuOpcije; // (11)

// --- STRUKTURA ZA PACIJENTA --- (2,3,4,15: tipovi, struct, typedef)
typedef struct {
    char ime[50];           // (5: snake_case)
    char prezime[50];
    int godine;             // (2: primitivni int)
    float rizik_postotak;   // (3: primitivni float)
    char tip_pregleda[30];  // (15: statički alocirana polja)
    char dijagnoza[100];
} Pacijent; // (4: typedef struct)

// --- DINAMIČKA LISTA PACIJENATA --- (12,13,16: pokazivači, strukture, dinamička memorija)
typedef struct {
    Pacijent* pacijenti;    // (12: pokazivač)
    int count;
    int capacity;
} PacijentList; // (4: typedef struct)

// --- ČVOR ZA POVEZANU LISTU --- (dopunski: dvostruko povezana lista)
typedef struct Node {
    Pacijent data;
    struct Node* next;
    struct Node* prev;
} Node;

// --- DVOSTRUKO POVEZANA LISTA --- (dopunski)
typedef struct {
    Node* head;
    Node* tail;
    int count;
} DvostrukaPovezanaLista;

// --- MAKRO FUNKCIJE --- (9: makro funkcije)
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

// --- GLOBALNE VARIJABLE --- (6,8: static, extern DEKLARACIJA - samo extern ovdje!)
extern PacijentList lista_pacijenata;           // (8: extern - samo deklaracija!)
extern DvostrukaPovezanaLista dvostuka_lista;   // (8: extern - samo deklaracija!)

// --- FUNKCIJE ZA UPRAVLJANJE LISTOM (lista.c) --- (16,17,18: dinamička memorija)
int inicijalizuj_listu(PacijentList* lista);
int prosliri_listu(PacijentList* lista);
int dodaj_pacijenta_u_listu(PacijentList* lista, const Pacijent* pacijent);
void oslobodi_listu(PacijentList* lista);

// --- FUNKCIJE ZA POVEZANU LISTU --- (dopunski)
int inicijalizuj_povezanu_listu(DvostrukaPovezanaLista* lista);
int dodaj_u_povezanu_listu(DvostrukaPovezanaLista* lista, const Pacijent* pacijent);
void prikazi_povezanu_listu(const DvostrukaPovezanaLista* lista);
void oslobodi_povezanu_listu(DvostrukaPovezanaLista* lista);

// --- FUNKCIJE ZA DATOTEKE (datoteke.c) --- (1,19: CRUD, datoteke)
void spremi_u_datoteku(const Pacijent* p);
int ucitaj_pacijente_iz_datoteke(PacijentList* lista);

// --- POMOĆNE FUNKCIJE (ui.c) --- (12,14: pokazivači, zaštita)
void ocisti_ekran(void);
int unos_broja(const char* pitanje);
char unos_dn(const char* pitanje);
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

#endif