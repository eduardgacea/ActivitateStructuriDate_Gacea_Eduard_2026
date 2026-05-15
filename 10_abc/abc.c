#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Carte Carte;
typedef struct Nod Nod;

struct Carte {
    char* titlu;
    int nrPagini;
};

struct Nod {
    Carte carte;
    Nod* st;
    Nod* dr;
};

Carte initializareCarte(const char* titlu, int nrPagini) {
    Carte carte = {0};
    
    const char* src = titlu == NULL ? "necunoscut" : titlu;
    carte.titlu = malloc(strlen(titlu) + 1);
    if(carte.titlu == NULL) return carte;
    strcpy(carte.titlu, src);

    carte.nrPagini = nrPagini;

    return carte;
}

Nod* initializareNod(Carte carte) {
    Nod* nod = malloc(sizeof(Nod));
    if(nod == NULL) return NULL;

    nod->carte = carte;
    nod->st = nod->dr = NULL;

    return nod;
}

void inserare(Nod** rad, Carte carte) {
    if(*rad == NULL) {
        Nod* nod = initializareNod(carte);
        *rad = nod;
    } else {
        if((*rad)->carte.nrPagini <= carte.nrPagini) {
            inserare(&(*rad)->dr, carte);
        }

        if((*rad)->carte.nrPagini > carte.nrPagini) {
            inserare(&(*rad)->st, carte);
        }

    }
}

void afisareCarte(Carte carte) {
    if(carte.titlu == NULL) {
        printf("titlu: necunoscut, nrPagini: %d\n", carte.nrPagini);
    } else {
        printf("titlu: %s, nrPagini: %d\n", carte.titlu, carte.nrPagini);
    }
}

void afisareArbore(Nod* rad) {
    if(rad) {
        afisareArbore(rad->st);
        afisareCarte(rad->carte);
        afisareArbore(rad->dr);
    }
}

void dezalocareArbore(Nod** rad) {
    if(*rad) {
        dezalocareArbore(&(*rad)->st);
        dezalocareArbore(&(*rad)->dr);

        free((*rad)->carte.titlu);
        free(*rad);
        *rad = NULL;
    }
}

Carte getCarteByNrPagini(Nod* rad, int nrPagini) {
    Carte carte = {0};

    if(rad) {
        if(rad->carte.nrPagini == nrPagini) {
            carte = rad->carte;
            carte.titlu = malloc(strlen(rad->carte.titlu) + 1);
            strcpy(carte.titlu, rad->carte.titlu);
            return carte;
        }

        if(rad->carte.nrPagini < nrPagini) {
            carte = getCarteByNrPagini(rad->dr, nrPagini);
        }

        if(rad->carte.nrPagini > nrPagini) {
            carte = getCarteByNrPagini(rad->st, nrPagini);
        }
    }

    return carte;
}

int main() {
    Nod* arbore = NULL;

    Carte c1 = initializareCarte("1984", 218);
    Carte c2 = initializareCarte("the hobbit", 104);
    Carte c3 = initializareCarte("crime and punishment", 650);
    Carte c4 = initializareCarte("the stranger", 187);
    Carte c5 = initializareCarte("a song of ice and fire", 796);

    inserare(&arbore, c1);
    inserare(&arbore, c2);
    inserare(&arbore, c3);
    inserare(&arbore, c4);
    inserare(&arbore, c5);
    
    afisareArbore(arbore);

    Carte c = getCarteByNrPagini(arbore, 218);
    afisareCarte(c);

    dezalocareArbore(&arbore);

    afisareArbore(arbore);

    return 0;
}