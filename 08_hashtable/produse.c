#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100

typedef struct Produs Produs;
typedef struct Nod Nod;
typedef struct Hashtable Hashtable;

struct Produs {
    char* nume;
    int cant;
    float pret;
};

struct Nod {
    Produs produs;
    Nod* next;
};

struct Hashtable {
    int dim;
    Nod** vector;
};

Produs citireProdusDinFisier(FILE* f) {
    Produs p = {0};
    if(f == NULL) return p;

    char buffer[BUFFER_SIZE];
    char sep[3] = ",\n";
    char* token;

    fgets(buffer, BUFFER_SIZE, f);

    token = strtok(buffer, sep);
    const char* src = token ? token : "necunoscut";
    p.nume = malloc(strlen(src) + 1);
    if(p.nume == NULL) return p;
    strcpy(p.nume, src);

    token = strtok(NULL, sep);
    p.cant = atoi(token);

    token = strtok(NULL, sep);
    p.pret = atof(token);

    return p;
}

void afisareProdus(Produs p) {
    if(p.nume == NULL) {
        printf("nume: necunoscut, cant: %d, pret: %.2f\n", p.cant, p.pret);
    } else {
        printf("nume: %s, cant: %d, pret: %.2f\n", p.nume, p.cant, p.pret);
    }
}

Hashtable initializareHashtable(int dim) {
    Hashtable ht = {0};
    if(dim <= 0) return ht;

    ht.dim = dim;
    ht.vector = malloc(dim * sizeof(Nod*));
    if(ht.vector == NULL) return ht;
     
    for(int i = 0 ; i < dim ; i++) {
        ht.vector[i] = NULL;
    }

    return ht;
}

int hash(int dim, int cant) {
    return cant % dim;
}

void inserare(Hashtable ht, Produs p) {
    if(ht.dim <= 0 || ht.vector == NULL) return;

    int poz = hash(ht.dim, p.cant);

    Nod* nodNou = malloc(sizeof(Nod));
    if(nodNou == NULL) return;

    nodNou->produs = p;
    nodNou->next = NULL;

    Nod* cap = ht.vector[poz];

    if(cap == NULL) {
        ht.vector[poz] = nodNou;
    } else {
        while(cap->next) {
            cap = cap->next;
        }

        cap->next = nodNou;
    }
}

Hashtable citireHashtableDinFisier(const char* numeFisier, int dim) {
    Hashtable ht = {0};
    if(numeFisier == NULL || dim <= 0) return ht;

    FILE* f = fopen(numeFisier, "r");
    if(f == NULL) return ht;

    ht = initializareHashtable(dim);

    while(!feof(f)) {
        inserare(ht, citireProdusDinFisier(f));
    }

    fclose(f);

    return ht;
}

void afisareHashTable(Hashtable ht) {
    if(ht.vector == NULL || ht.dim == 0) {
        printf("hashtable neinitializat\n");
        return;
    }

    for(int i = 0 ; i < ht.dim ; i++) {
        printf("=== LISTA %d===\n", i);
        Nod* cap = ht.vector[i];
        while(cap) {
            afisareProdus(cap->produs);
            cap = cap->next;
        }
    }
    printf("-----------------------\n");
}

Produs getProdusDupaCantitate(Hashtable ht, int cant) {
    Produs p = {0};
    if(ht.vector == NULL) return p;

    int poz = hash(ht.dim, cant);
    
    if(0 <= poz && poz < ht.dim) {
        Nod* cap = ht.vector[poz];
        if(cap == NULL) return p;

        while(cap) {
            if(cap->produs.cant == cant) {
                p = cap->produs;
                p.nume = malloc(strlen(cap->produs.nume) + 1);
                if(p.nume == NULL) return p;
                strcpy(p.nume, cap->produs.nume);
                return p;
            }
            cap = cap->next;
        }
    }

    return p;
}

void dealocareHashtable(Hashtable* ht) {
    if(ht == NULL || ht->dim <= 0 || ht->vector == NULL) return;

    for(int i = 0 ; i < ht->dim ; i++) {
        Nod* cap = ht->vector[i];
        while(cap) {
            Nod* aux = cap->next;
            free(cap->produs.nume);
            free(cap);
            cap = aux;
        }
    }

    free(ht->vector);
    ht->vector = NULL;
}

int main() {
    Hashtable ht = citireHashtableDinFisier("produse.txt", 4);
    Produs p = getProdusDupaCantitate(ht, 35);

    afisareHashTable(ht);
    printf("produsul cautat este: ");
    afisareProdus(p);

    dealocareHashtable(&ht);

    afisareHashTable(ht);

    return 0;
}