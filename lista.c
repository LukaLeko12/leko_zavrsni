#define _CRT_SECURE_NO_WARNINGS
#include "pacijent.h"

// --- INICIJALIZACIJA LISTE ---
int inicijalizuj_listu(PacijentList* lista) {
    if (lista == NULL) return 0;

    lista->capacity = INITIAL_CAPACITY;
    lista->count = 0;
    lista->pacijenti = (Pacijent*)malloc(lista->capacity * sizeof(Pacijent));
    PROVJERA_MEMORIJE(lista->pacijenti);

    return 1;
}

// --- PROŠIRENJE LISTE ---
int prosliri_listu(PacijentList* lista) {
    if (lista == NULL) return 0;

    int nova_kapaciteta = lista->capacity * 2;
    Pacijent* temp = (Pacijent*)realloc(lista->pacijenti,
        nova_kapaciteta * sizeof(Pacijent));
    PROVJERA_MEMORIJE(temp);
    lista->pacijenti = temp;
    lista->capacity = nova_kapaciteta;

    printf("[*] Lista prosirena na kapacitet: %d\n", nova_kapaciteta);
    return 1;
}

// --- DODAJ PACIJENTA U LISTU ---
int dodaj_pacijenta_u_listu(PacijentList* lista, const Pacijent* pacijent) {
    if (lista == NULL || pacijent == NULL) return 0;

    if (lista->count >= lista->capacity) {
        if (!prosliri_listu(lista)) return 0;
    }

    lista->pacijenti[lista->count] = *pacijent;
    lista->count++;

    return 1;
}

// --- OSLOBAĐANJE MEMORIJE ---
void oslobodi_listu(PacijentList* lista) {
    if (lista == NULL) return;

    if (lista->pacijenti != NULL) {
        free(lista->pacijenti);
        lista->pacijenti = NULL;
    }

    lista->count = 0;
    lista->capacity = 0;
}