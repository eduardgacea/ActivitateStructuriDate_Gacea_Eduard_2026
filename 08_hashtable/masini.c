#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Masina Masina;
typedef struct HashTable HashTable;
typedef struct Nod Nod;

struct Masina {
	int id;
	int nrUsi;
	float pret;
	char *model;
	char *numeSofer;
	unsigned char serie;
};

struct HashTable {
    Nod **vector;
    int dim;
};

struct Nod {
    Masina *masina;
    Nod *next;
};

// allocation
Masina *citireMasinaDinFisier(FILE *file);
Nod *initializareNod(Masina *masina);
HashTable *initializareHashTable(int dim);
HashTable *citireHashTableDinFisier(const char *numeFisier, int dim);

// api
int hash(int id, int dim);
void inserareSfarsit(Nod **cap, Masina *masina);
void inserareHashTable(HashTable *ht, Masina *masina);

// logging
void afisareMasina(Masina *masina);
void afisareLista(Nod* cap);
void afisareHashTable(HashTable *ht);

// deallocation

int main() {
    HashTable *ht = citireHashTableDinFisier("masini.txt", 5);
    afisareHashTable(ht);

    return 0;
}

// allocation
Masina *citireMasinaDinFisier(FILE *file) {
    if(file == NULL) return NULL;

    Masina *masina = malloc(sizeof(Masina));
    if(masina == NULL) return NULL;

    char buffer[100];
    char sep[3] = ",\n";
    char *token;

    fgets(buffer, 100, file);

    token = strtok(buffer, sep);
    masina->id = atoi(token);
    
    token = strtok(NULL, sep);
    masina->nrUsi = atoi(token);

    token = strtok(NULL, sep);
    masina->pret = atof(token);

    token = strtok(NULL, sep);
    char *srcModel = token == NULL ? "necunoscut" : token;
    masina->model = malloc(strlen(srcModel) + 1);
    if(masina->model == NULL) {
        return masina;
    }
    strcpy(masina->model, srcModel);

    token = strtok(NULL, sep);
    char *srcSofer = token == NULL ? "necunoscut" : token;
    masina->numeSofer = malloc(strlen(srcSofer) + 1);
    if(masina->numeSofer == NULL) {
        free(masina->model);
        masina->model = NULL;
        return masina;
    }
    strcpy(masina->numeSofer, srcSofer);

    token = strtok(NULL, sep);
    masina->serie = *token;

    return masina;
}

Nod *initializareNod(Masina *masina) {
    if(masina == NULL) return NULL;

    Nod *nod = malloc(sizeof(Nod));
    if(nod == NULL) return NULL;
    
    nod->masina = masina;
    nod->next = NULL;

    return nod;
}

HashTable *initializareHashTable(int dim) {
    if(dim <= 0) return NULL;

    HashTable *ht = malloc(sizeof(HashTable));
    if(ht == NULL) return NULL;

    ht->dim = dim;
    ht->vector = malloc(dim * sizeof(Nod *));
    if(ht->vector == NULL) return NULL;

    for(int i = 0 ; i < ht->dim ; i++) {
        ht->vector[i] = NULL;
    }

    return ht;
}

HashTable *citireHashTableDinFisier(const char *numeFisier, int dim) {
    FILE *file = fopen(numeFisier, "r");
    if(file == NULL) return NULL;

    HashTable *ht = initializareHashTable(dim);
    if(ht == NULL) return NULL;
    
    while(!feof(file)) {
        inserareHashTable(ht, citireMasinaDinFisier(file));
    }

    fclose(file);

    return ht;
}

// api
int hash(int id, int dim) {
    return id % dim;
}

void inserareSfarsit(Nod **cap, Masina *masina) {
    if(cap == NULL || masina == NULL) return;

    Nod *nodNou = initializareNod(masina);
    
    if(*cap == NULL) {
        *cap = nodNou;
    } else {
        Nod *aux = *cap;
        while(aux->next) {
            aux = aux->next;
        }
        aux->next = nodNou;
    }

}

void inserareHashTable(HashTable *ht, Masina *masina) {
    if(ht == NULL || ht->vector == NULL || ht->dim == 0 || masina == NULL) return;

    int poz = hash(masina->id, ht->dim);
    Nod **cap = &(ht->vector[poz]);

    inserareSfarsit(cap, masina);
}

// logging
void afisareMasina(Masina *masina) {
    if(masina == NULL) {
        printf("masina neinitializata\n");
        return;
    }

    printf("id: %d\n", masina->id);
	printf("nr usi : %d\n", masina->nrUsi);
	printf("pret: %.2f\n", masina->pret);
    
    if(masina->model == NULL) {
        printf("model: necunoscut\n");
    } else {
        printf("model: %s\n", masina->model);
    }
	
    if(masina->numeSofer == NULL) {
        printf("nume sofer: necunoscut\n");
    } else {
        printf("nume sofer: %s\n", masina->numeSofer);
    }
	
	printf("serie: %c\n\n", masina->serie);
}

void afisareLista(Nod* cap) {
    if(cap == NULL) {
        printf("lista goala\n");
        return;
    }

    Nod *n = cap;
    while(n) {
        afisareMasina(n->masina);
        n = n->next;
    }
}

void afisareHashTable(HashTable *ht) {
    if(ht == NULL || ht->vector == NULL) {
        printf("hashtable neinitializat\n");
        return;
    }

    if(ht->dim == 0) {
        printf("hashtable gol\n");
        return;
    }

    for(int i = 0 ; i < ht->dim ; i++) {
        printf("=== LISTA %d ===\n", i);
        afisareLista(ht->vector[i]);
    }
}

// deallocation