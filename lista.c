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

// --- INICIJALIZACIJA POVEZANE LISTE ---
int inicijalizuj_povezanu_listu(DvostrukaPovezanaLista* lista) {
    if (lista == NULL) return 0; 
    lista->head = NULL;
    lista->tail = NULL;
    lista->count = 0;
    return 1;
}

// --- DODAJ U POVEZANU LISTU --- 
int dodaj_u_povezanu_listu(DvostrukaPovezanaLista* lista, const Pacijent* pacijent) {
    if (lista == NULL || pacijent == NULL) return 0; 

    Node* novi_cvor = (Node*)malloc(sizeof(Node)); 
    PROVJERA_MEMORIJE(novi_cvor); 

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

// --- PRETRAGA U POVEZANOJ LISTI ---
Node* pretraga_u_povezanoj_listi(DvostrukaPovezanaLista* lista, const char* prezime) {
    if (lista == NULL || prezime == NULL) return NULL;

    Node* trenutni = lista->head;
    while (trenutni != NULL) {
        if (strcmp(trenutni->data.prezime, prezime) == 0) {
            return trenutni;
        }
        trenutni = trenutni->next;
    }
    return NULL; 
}

// --- BRISANJE IZ POVEZANE LISTE --- 
int obrisi_iz_povezane_liste(DvostrukaPovezanaLista* lista, const char* prezime) {
    if (lista == NULL || prezime == NULL) return 0;

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

    free(cvor); 
    cvor = NULL; 
    lista->count--;

    return 1;
}

// --- PRIKAZI POVEZANU LISTU --- 
void prikazi_povezanu_listu(const DvostrukaPovezanaLista* lista) {
    if (lista == NULL || lista->head == NULL) {
        printf(" Povezana lista je prazna.\n");
        return;
    }

    printf("\n---- PRIKAZ DVOSTRUKE POVEZANE LISTE ----\n");
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

// --- OSLOBOĐENJE POVEZANE LISTE --- 
void oslobodi_povezanu_listu(DvostrukaPovezanaLista* lista) {
    if (lista == NULL) return; 

    Node* trenutni = lista->head;
    while (trenutni != NULL) {
        Node* temp = trenutni;
        trenutni = trenutni->next;
        free(temp);
        temp = NULL; 
    }

    lista->head = NULL; 
    lista->tail = NULL;
    lista->count = 0;
}