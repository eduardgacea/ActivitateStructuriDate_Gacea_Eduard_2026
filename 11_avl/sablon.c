#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100

typedef struct Masina Masina;
typedef struct Nod Nod;

struct Masina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

struct Nod {
    Masina masina;
    Nod* st;
    Nod* dr;
};

Masina citireMasinaDinFisier(FILE* f) {
    Masina m = {0};
    if(f == NULL) return m;

    char buffer[BUFFER_SIZE];
    char sep[3] = ",\n";
    char* token;

    fgets(buffer, BUFFER_SIZE, f);

    token = strtok(buffer, sep);
    m.id = atoi(token);

    token = strtok(NULL, sep);
    m.nrUsi = atoi(token);

    token = strtok(NULL, sep);
    m.pret = atof(token);

    token = strtok(NULL, sep);
    const char* srcModel = token ? token : "necunoscut";
    m.model = malloc(strlen(srcModel) + 1);
    if(m.model == NULL) return m;
    strcpy(m.model, srcModel);

    token = strtok(NULL, sep);
    const char* srcSofer = token ? token : "necunoscut";
    m.numeSofer = malloc(strlen(srcSofer) + 1);
    if(m.numeSofer == NULL) {
        free(m.model);
        m.model = NULL;
        return m;
    }
    strcpy(m.numeSofer, srcSofer);

    token = strtok(NULL, sep);
    m.serie = *token;

    return m;
}

Nod* initializareNod(Masina m) {
    Nod* n = malloc(sizeof(Nod));
    if(n == NULL) return NULL;

    n->masina = m;
    n->st = n->dr = NULL;

    return n;
}

void afisareMasina(Masina m) {
	printf("Id: %d\n", m.id);
	printf("Nr. usi : %d\n", m.nrUsi);
	printf("Pret: %.2f\n", m.pret);
	printf("Model: %s\n", m.model);
	printf("Nume sofer: %s\n", m.numeSofer);
	printf("Serie: %c\n\n", m.serie);
}

void afisareArbore(Nod* rad) {
    if(rad) {
        afisareArbore(rad->st);
        afisareMasina(rad->masina);
        afisareArbore(rad->dr);
    }
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

int getDiferentaInaltime(Nod* rad) {
    if(rad) {
        return getInaltimeArbore(rad->st) - getInaltimeArbore(rad->dr);
    }

    return 0;
}

void rotatieStanga(Nod** rad) {
    Nod* aux = (*rad)->dr;
    (*rad)->dr = aux->st;
    aux->st = (*rad);
    (*rad) = aux;
}

void rotatieDreapta(Nod** rad) {
    Nod* aux = (*rad)->st;
    (*rad)->st = aux->dr;
    aux->dr = (*rad);
    (*rad) = aux;
}

void inserare(Nod** rad, Masina m) {
    if(*rad == NULL) {
        Nod* n = initializareNod(m);
        *rad = n;
    } else {
        if(m.id < (*rad)->masina.id) {
            inserare(&(*rad)->st, m);
        }

        if(m.id > (*rad)->masina.id) {
            inserare(&(*rad)->dr, m);
        }
    }

    int diferentaInaltime = getDiferentaInaltime(*rad);

    if(diferentaInaltime == 2) {
        // dezechilibru stanga
        if(getDiferentaInaltime((*rad)->st) == -1) {
            rotatieStanga(&(*rad)->st);
        }
        rotatieDreapta(rad);
    }

    if(diferentaInaltime == -2) {
        // dezechilibru dreapta
        if(getDiferentaInaltime((*rad)->dr) == 1) {
            rotatieDreapta(&(*rad)->dr);
        }
        rotatieStanga(rad);
    }
}

Nod* citireArboreDinFisier(const char* numeFisier) {
    if(numeFisier == NULL) return NULL;

    FILE* f = fopen(numeFisier, "r");
    if(f == NULL) return NULL;

    Nod* arbore = NULL;

    while(!feof(f)) {
        inserare(&arbore, citireMasinaDinFisier(f));
    }

    fclose(f);

    return arbore;
}

void dezalocareAbore(Nod** rad) {
    if(*rad) {
        dezalocareAbore(&(*rad)->st);
        dezalocareAbore(&(*rad)->dr);

        free((*rad)->masina.model);
        free((*rad)->masina.numeSofer);

        free(*rad);
        *rad = NULL;
    }
}

Masina getMasinaById(Nod* rad, int id) {
    Masina m = {0};

    if(rad) {
        if(id == rad->masina.id) {
            m = rad->masina;
            
            m.model = malloc(strlen(rad->masina.model) + 1);
            if(m.model == NULL) return m;
            strcpy(m.model, rad->masina.model);

            m.numeSofer = malloc(strlen(rad->masina.numeSofer) + 1);
            if(m.numeSofer == NULL) {
                free(m.model);
                m.model = NULL;
                return m;
            }
            strcpy(m.numeSofer, rad->masina.numeSofer);
            
            return m;
        }

        if(id < rad->masina.id) {
            return getMasinaById(rad->st, id);
        }

        if(id > rad->masina.id){
            return getMasinaById(rad->dr, id);
        }
    }

    return m;
}

int getNumarNoduri(Nod* rad) {
    if(rad) {
        return 1 + getNumarNoduri(rad->st) + getNumarNoduri(rad->dr);
    }

    return 0;
}

float getPretTotal(Nod* rad) {
    if(rad) {
        return rad->masina.pret + getPretTotal(rad->st) + getPretTotal(rad->dr);
    }

    return 0;
}

float getPretMasiniSofer(Nod* rad, const char* numeSofer) {
    if(rad) {
        float suma = getPretMasiniSofer(rad->st, numeSofer) + getPretMasiniSofer(rad->dr, numeSofer);
        if(strcmp(rad->masina.numeSofer, numeSofer) == 0) {
            suma += rad->masina.pret;
        }

        return suma;
    }

    return 0;
}

int main() {
    Nod* arbore = citireArboreDinFisier("masini.txt");
    Masina m = getMasinaById(arbore, 4);
    int inaltime = getInaltimeArbore(arbore);
    int nrNoduri = getNumarNoduri(arbore);
    float pretTotal = getPretTotal(arbore);
    float pretMasiniIonescu = getPretMasiniSofer(arbore, "Ionescu");

    afisareArbore(arbore);
    printf("------------------------------------\n");
    printf("masina cautata este:\n");
    afisareMasina(m);
    printf("inaltime arbore: %d\n", inaltime);
    printf("numar noduri: %d\n", nrNoduri);
    printf("pret total: %.2f\n", pretTotal);
    printf("pretul masinilor soferului Ionescu: %.2f\n", pretMasiniIonescu);

    dezalocareAbore(&arbore);
    afisareArbore(arbore);

    return 0;
}