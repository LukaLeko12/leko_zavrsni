#ifndef PACIJENT_H
#define PACIJENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

// --- KONSTANTE ---
#define INITIAL_CAPACITY 20

// --- ENUM ZA IZBORNIK ---
typedef enum {
    MELANOM = 1,
    PLUCA = 2,
    PROSTATA = 3,
    PRIKAZI = 4,
    STATISTIKA = 5,
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
} Pacijent;

// --- DINAMIČKA LISTA PACIJENATA ---
typedef struct {
    Pacijent* pacijenti;
    int count;
    int capacity;
} PacijentList;

// --- MAKRO FUNKCIJE ---
#define PROVJERA_MEMORIJE(ptr) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "[!] GRESKA: Alokacija memorije neuspjesna!\n"); \
            perror("malloc"); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)

// --- GLOBALNA LISTA ---
extern PacijentList lista_pacijenata;

// --- FUNKCIJE ZA UPRAVLJANJE LISTOM (lista.c) ---
int inicijalizuj_listu(PacijentList* lista);
int prosliri_listu(PacijentList* lista);
int dodaj_pacijenta_u_listu(PacijentList* lista, const Pacijent* pacijent);
void oslobodi_listu(PacijentList* lista);

// --- FUNKCIJE ZA DATOTEKE (datoteke.c) ---
void spremi_u_datoteku(const Pacijent* p);
int ucitaj_pacijente_iz_datoteke(PacijentList* lista);

// --- POMOĆNE FUNKCIJE (ui.c) ---
void ocisti_ekran(void);
int unos_broja(const char* pitanje);
char unos_dn(const char* pitanje);
void unos_string(char* buffer, int max_size, const char* prompt);

// --- MODULI PREGLEDA (pacijent.c) ---
void procjena_melanoma(void);
void procjena_pluca(void);
void procjena_prostate(void);

// --- PRIKAZI PODATKE (ui.c) ---
void prikazi_sve_pacijente(void);
void prikazi_statistiku(void);
void prikazi_meni(void);

#endif