#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Produs Produs;
typedef struct Nod Nod; 

struct Produs {
    char* nume;
    int cantitate;
    float pret;
};

struct Nod {
    Produs produs;
    Nod* st;
    Nod* dr;
};

Produs citireProdusDinFisier(FILE* f) {
    Produs p = {0};
    if(f == NULL) return p;

    char buffer[100];
    char sep[3] = ",\n";
    char* token;

    fgets(buffer, 100, f);

    token = strtok(buffer, sep);
    const char* src = token ? token : "necunoscut";
    p.nume = malloc(strlen(src) + 1);
    if(p.nume == NULL) return p;
    strcpy(p.nume, src);

    token = strtok(NULL, sep);
    p.cantitate = atoi(token);

    token = strtok(NULL, sep);
    p.pret = atof(token);

    return p;
}

Nod* initializareNod(Produs p) {
    Nod* n = malloc(sizeof(Nod));
    if(n == NULL) return NULL;

    n->produs = p;
    n->st = n->dr = NULL;

    return n;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int getInaltimeArbore(Nod* rad) {
    if(rad) {
        return 1 + max(getInaltimeArbore(rad->st), getInaltimeArbore(rad->dr));
    }

    return 0;
}

int getDiferenteInaltimi(Nod* rad) {
    if(rad) {
        return getInaltimeArbore(rad->st) - getInaltimeArbore(rad->dr);
    }

    return 0;
}

void rotireLaStanga(Nod** rad) {
    Nod* aux = (*rad)->dr;
    (*rad)->dr = aux->st;
    aux->st = (*rad);
    (*rad) = aux;
}

void rotireLaDreapta(Nod** rad) {
    Nod* aux = (*rad)->st;
    (*rad)->st = aux->dr;
    aux->dr = (*rad);
    (*rad) = aux;
}

void inserare(Nod** rad, Produs p) {
    if(*rad == NULL) {
        Nod* n = initializareNod(p);
        *rad = n;
    } else {
        if(p.cantitate < (*rad)->produs.cantitate) {
            inserare(&(*rad)->st, p);
        }

        if(p.cantitate > (*rad)->produs.cantitate) {
            inserare(&(*rad)->dr, p);
        }
    }

    int diferentaInaltimi = getDiferenteInaltimi(*rad);

    // dezechilibru stanga
    if(diferentaInaltimi == 2) {
        if(getDiferenteInaltimi((*rad)->st) == -1) {
            rotireLaStanga(&(*rad)->st);
        }
        rotireLaDreapta(rad);
    }

    // dezechilibru dreapta
    if(diferentaInaltimi == -2) {
        if(getDiferenteInaltimi((*rad)->dr) == 1){
            rotireLaDreapta(&(*rad)->dr);
        }
        rotireLaStanga(rad);
    }
}

void afisareProdus(Produs p) {
    if(p.nume == NULL) {
        printf("nume: necunoscut, cantitate: %d, pret: %.2f\n", p.cantitate, p.pret);
    } else {
        printf("nume: %s, cantitate: %d, pret: %.2f\n", p.nume, p.cantitate, p.pret);
    }
}

void afisareArbore(Nod* rad) {
    if(rad) {
        afisareArbore(rad->st);
        afisareProdus(rad->produs);
        afisareArbore(rad->dr);
    }
}

Nod* initializareArboreDinFisier(const char* numeFisier) {
    if(numeFisier == NULL) return NULL;

    FILE* f = fopen(numeFisier, "r");
    if(f == NULL) return NULL;

    Nod* arbore = NULL;

    while(!feof(f)) {
        inserare(&arbore, citireProdusDinFisier(f));
    }

    fclose(f);

    return arbore;
}

void dezalocareArbore(Nod** rad) {
    if(*rad) {
        dezalocareArbore(&(*rad)->st);
        dezalocareArbore(&(*rad)->dr);

        free((*rad)->produs.nume);
        free(*rad);
        *rad = NULL;
    }
}

Produs getProdusByCantitate(Nod* rad, int cantitate) {
    Produs p = {0};
    
    if(rad) {
        if(cantitate == rad->produs.cantitate) {
            p = rad->produs;
            const char* src = rad->produs.nume ? rad->produs.nume : "necunoscut";
            p.nume = malloc(strlen(src) + 1);
            if(p.nume == NULL) return p;
            strcpy(p.nume, src);
            return p;
        }

        if(cantitate < rad->produs.cantitate) {
            return getProdusByCantitate(rad->st, cantitate);
        }

        if(cantitate > rad->produs.cantitate) {
            return getProdusByCantitate(rad->dr, cantitate);
        }

    }

    return p;
}

int getNumarNoduri(Nod* rad) {
    if(rad) {
        return 1 + getNumarNoduri(rad->st) + getNumarNoduri(rad->dr);
    }

    return 0;
}

float getSumaPreturi(Nod* rad) {
    if(rad) {
        return rad->produs.pret + getSumaPreturi(rad->st) + getSumaPreturi(rad->dr);
    }

    return 0;
}

float getValoareNod(Nod* nod) {
    return nod->produs.cantitate * nod->produs.pret;
}

float getValoareStoc(Nod* rad) {
    if(rad) {
        return getValoareNod(rad) + getValoareStoc(rad->st) + getValoareStoc(rad->dr);
    }

    return 0;
}

int main() {
    Nod* arbore = initializareArboreDinFisier("produse.txt");
    Produs p = getProdusByCantitate(arbore, 14);
    int nrNoduri = getNumarNoduri(arbore);
    int inaltime = getInaltimeArbore(arbore);
    float pretMediu = getSumaPreturi(arbore) / (float)getNumarNoduri(arbore);
    float valoareStoc = getValoareStoc(arbore);

    afisareArbore(arbore);
    printf("\nprodusul cautat este: ");
    afisareProdus(p);
    printf("\narborele are un numar total de %d noduri\n", nrNoduri);
    printf("\narborele are inaltimea: %d\n", inaltime);
    printf("\npretul mediue este: %.2f\n", pretMediu);
    printf("\nvaloarea totala a stocului este: %.2f\n", valoareStoc);

    dezalocareArbore(&arbore);

    afisareArbore(arbore);

    return 0;
}