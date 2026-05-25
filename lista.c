// lista.c -- UPRAVLJANJE DINAMIČKOM LISTOM
#define _CRT_SECURE_NO_WARNINGS
#include "pacijent.h"

// --- INICIJALIZACIJA LISTE --- (16,17: dinamička memorija, malloc)
int inicijalizuj_listu(PacijentList* lista) {
    if (lista == NULL) return 0; // (14: zaštita parametara)

    lista->capacity = INITIAL_CAPACITY;
    lista->count = 0;
    lista->pacijenti = (Pacijent*)malloc(lista->capacity * sizeof(Pacijent)); // (16,17: malloc)
    PROVJERA_MEMORIJE(lista->pacijenti); // (9,18: makro, provjera)

    return 1;
}

// --- PROŠIRENJE LISTE --- (17: realloc - dinamičko proširenje)
int prosliri_listu(PacijentList* lista) {
    if (lista == NULL) return 0; // (14)

    int nova_kapaciteta = lista->capacity * 2;
    Pacijent* temp = (Pacijent*)realloc(lista->pacijenti,
        nova_kapaciteta * sizeof(Pacijent)); // (17: realloc)
    PROVJERA_MEMORIJE(temp); // (9,18)
    lista->pacijenti = temp;
    lista->capacity = nova_kapaciteta;

    return 1;
}

// --- DODAJ PACIJENTA U LISTU --- (1: INSERT - CREATE, 12: pokazivači)
int dodaj_pacijenta_u_listu(PacijentList* lista, const Pacijent* pacijent) {
    if (lista == NULL || pacijent == NULL) return 0; // (14: zaštita)

    if (lista->count >= lista->capacity) {
        if (!prosliri_listu(lista)) return 0;
    }

    lista->pacijenti[lista->count] = *pacijent;
    lista->count++;

    return 1;
}

// --- OSLOBAĐANJE MEMORIJE --- (18: sigurno brisanje, free, NULL)
void oslobodi_listu(PacijentList* lista) {
    if (lista == NULL) return; // (14)

    if (lista->pacijenti != NULL) {
        free(lista->pacijenti); // (17,18: free)
        lista->pacijenti = NULL; // (18: anuliranje pokazivača)
    }

    lista->count = 0;
    lista->capacity = 0;
}

// === POVEZANA LISTA === (dopunski)

// --- INICIJALIZACIJA POVEZANE LISTE ---
int inicijalizuj_povezanu_listu(DvostrukaPovezanaLista* lista) {
    if (lista == NULL) return 0; // (14)
    lista->head = NULL;
    lista->tail = NULL;
    lista->count = 0;
    return 1;
}

// --- DODAJ U POVEZANU LISTU --- (dopunski, 12: pokazivači, 16,17: malloc)
int dodaj_u_povezanu_listu(DvostrukaPovezanaLista* lista, const Pacijent* pacijent) {
    if (lista == NULL || pacijent == NULL) return 0; // (14)

    Node* novi_cvor = (Node*)malloc(sizeof(Node)); // (17: malloc)
    PROVJERA_MEMORIJE(novi_cvor); // (9,18)

    novi_cvor->data = *pacijent;
    novi_cvor->next = NULL;
    novi_cvor->prev = lista->tail;

    if (lista->tail != NULL) {
        lista->tail->next = novi_cvor;
    }
    lista->tail = novi_cvor;

    if (lista->head == NULL) {
        lista->head = novi_cvor;
    }

    lista->count++;
    return 1;
}

// --- PRETRAGA U POVEZANOJ LISTI --- (24: pretraživanje)
Node* pretraga_u_povezanoj_listi(DvostrukaPovezanaLista* lista, const char* prezime) {
    if (lista == NULL || prezime == NULL) return NULL; // (14)

    Node* trenutni = lista->head;
    while (trenutni != NULL) {
        if (strcmp(trenutni->data.prezime, prezime) == 0) {
            return trenutni; // (24: pronašao)
        }
        trenutni = trenutni->next;
    }
    return NULL; // (24: nije pronašao)
}

// --- BRISANJE IZ POVEZANE LISTE --- (1: DELETE, dopunski)
int obrisi_iz_povezane_liste(DvostrukaPovezanaLista* lista, const char* prezime) {
    if (lista == NULL || prezime == NULL) return 0; // (14)

    Node* cvor = pretraga_u_povezanoj_listi(lista, prezime);
    if (cvor == NULL) return 0;

    if (cvor->prev != NULL) {
        cvor->prev->next = cvor->next;
    }
    else {
        lista->head = cvor->next;
    }

    if (cvor->next != NULL) {
        cvor->next->prev = cvor->prev;
    }
    else {
        lista->tail = cvor->prev;
    }

    free(cvor); // (18: free)
    cvor = NULL; // (18: anuliranje)
    lista->count--;

    return 1;
}

// --- PRIKAZI POVEZANU LISTU --- (1: READ - dopunski)
void prikazi_povezanu_listu(const DvostrukaPovezanaLista* lista) {
    if (lista == NULL || lista->head == NULL) {
        printf(" Povezana lista je prazna.\n");
        return;
    }

    printf("\n=== PRIKAZ DVOSTRUKE POVEZANE LISTE ===\n");
    printf("%-12s %-12s %-4s %-15s %-8s %-30s\n",
        "IME", "PREZIME", "GOD", "TIP", "RIZIK", "DIJAGNOZA");
    printf("---------------------------------------------------------------\n");

    Node* trenutni = lista->head;
    while (trenutni != NULL) {
        printf("%-12s %-12s %-4d %-15s %-7.2f%% %-30s\n",
            trenutni->data.ime,
            trenutni->data.prezime,
            trenutni->data.godine,
            trenutni->data.tip_pregleda,
            trenutni->data.rizik_postotak,
            trenutni->data.dijagnoza);
        trenutni = trenutni->next;
    }
}

// --- OSLOBOĐENJE POVEZANE LISTE --- (18: sigurno brisanje, free, NULL)
void oslobodi_povezanu_listu(DvostrukaPovezanaLista* lista) {
    if (lista == NULL) return; // (14)

    Node* trenutni = lista->head;
    while (trenutni != NULL) {
        Node* temp = trenutni;
        trenutni = trenutni->next;
        free(temp); // (18: free)
        temp = NULL; // (18: anuliranje)
    }

    lista->head = NULL; // (18: anuliranje)
    lista->tail = NULL;
    lista->count = 0;
}