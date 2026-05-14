#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Masina Masina;
typedef struct Heap Heap;

struct Masina {
    int id;
    int nrUsi;
    float pret;
    char *model;
    char *numeSofer;
    unsigned char serie;
};

struct Heap {
    Masina *masini;
    int lungime;
    int nrElemente;
};

// allocators
Masina citireMasinaDinFisier(FILE *file);
Heap initializareHeap(int lungime);

// api
void filtreazaHeap(Heap heap, int pozitieNod);
Heap citireHeapDeMasiniDinFisier(const char* numeFisier);
Masina extrageMasina(Heap *heap);

// logging
void afisareMasina(Masina masina);
void afisareHeap(Heap heap);
void afisareHeapAscuns(Heap heap);

// deallocators
void dezalocareHeap(Heap *heap);

int main()
{
    Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
    afisareHeap(heap);

    Masina masinaPretMinim = extrageMasina(&heap);
    printf("masina pret minim:\n");
    afisareMasina(masinaPretMinim);

    afisareHeap(heap);

    dezalocareHeap(&heap);
    afisareHeap(heap);

    return 0;
}

// allocators
Masina citireMasinaDinFisier(FILE *file) {
    Masina masina = {0};
    char buffer[100];
    char sep[3] = ",\n";
    char *token;
    
    fgets(buffer, 100, file);

    token = strtok(buffer, sep);
    masina.id = atoi(token);
    
    token = strtok(NULL, sep);
    masina.nrUsi = atoi(token);

    token = strtok(NULL, sep);
    masina.pret = atof(token);

    token = strtok(NULL, sep);
    const char *srcModel = token ? token : "necunoscut";
    masina.model = malloc(strlen(srcModel) + 1);
    if(masina.model == NULL) return masina;
    strcpy(masina.model, srcModel);

    token = strtok(NULL, sep);
    const char *srcSofer = token ? token : "necunoscut";
    masina.numeSofer = malloc(strlen(srcSofer) + 1);
    if(masina.numeSofer == NULL) {
        free(masina.model);
        masina.model = NULL;
        return masina;
    }
    strcpy(masina.numeSofer, srcSofer);

    token = strtok(NULL, sep);
    masina.serie = *token;

    return masina;
}

Heap initializareHeap(int lungime) {
    Heap heap = {0};
    if(lungime <= 0) return heap;
    
    heap.lungime = lungime;
    heap.nrElemente = 0;
    
    heap.masini = malloc(lungime * sizeof(Masina));
    if(heap.masini == NULL) return heap;
    
    return heap;
}

// api
void filtreazaHeap(Heap heap, int pozitieNod) {
    int indexCopilStanga = pozitieNod * 2 + 1;
    int indexCopilDreapta = pozitieNod * 2 + 2;
    int indexElementMinim = pozitieNod;

    if(indexCopilStanga < heap.nrElemente && heap.masini[indexCopilStanga].pret < heap.masini[indexElementMinim].pret) {
        indexElementMinim = indexCopilStanga;
    }

    if(indexCopilDreapta < heap.nrElemente && heap.masini[indexCopilDreapta].pret < heap.masini[indexElementMinim].pret) {
        indexElementMinim = indexCopilDreapta;
    }

    if(indexElementMinim != pozitieNod) {
        Masina aux = heap.masini[pozitieNod];
        heap.masini[pozitieNod] = heap.masini[indexElementMinim];
        heap.masini[indexElementMinim] = aux;
        if(indexElementMinim < (heap.nrElemente - 2) / 2) {
            filtreazaHeap(heap, indexElementMinim);
        }
    }
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
    Heap heap = initializareHeap(10);
    if(numeFisier == NULL) return heap;

    FILE *file = fopen(numeFisier, "r");
    if(file == NULL) return heap;

    while(!feof(file)) {
        heap.masini[heap.nrElemente++] = citireMasinaDinFisier(file);
    }

    fclose(file);

    for(int i = (heap.nrElemente - 2) / 2 ; i >= 0 ; i--) {
        filtreazaHeap(heap, i);
    }
    
    return heap;
}

Masina extrageMasina(Heap *heap) {
    Masina masina = {0};

    if(heap->nrElemente > 0) {
        masina = heap->masini[0];
        heap->masini[0] = heap->masini[heap->nrElemente - 1];
        heap->masini[heap->nrElemente - 1] = masina;

        heap->nrElemente--;

        for(int i = (heap->nrElemente - 2) / 2 ; i >= 0; i--) {
            filtreazaHeap(*heap, i);
        }
    }

    return masina;
}

// logging
void afisareMasina(Masina masina) {
    printf("id: %d\n", masina.id);
	printf("nr usi : %d\n", masina.nrUsi);
	printf("pret: %.2f\n", masina.pret);
    
    if(masina.model == NULL) {
        printf("model: necunoscut");
    } else {
        printf("model: %s\n", masina.model);
    }
	
    if(masina.numeSofer == NULL) {
        printf("nume sofer: necunoscut");
    } else {
        printf("nume sofer: %s\n", masina.numeSofer);
    }
	
	printf("serie: %c\n\n", masina.serie);
}

void afisareHeap(Heap heap) {
    if(heap.masini == NULL) {
        printf("heap neinitializat\n");
        return;
    }

    if(heap.nrElemente == 0) {
        printf("heap gol\n");
        return;
    }

    for(int i = 0 ; i < heap.nrElemente ; i++) {
        afisareMasina(heap.masini[i]);
    }
}

void afisareHeapAscuns(Heap heap) {
    if(heap.masini == NULL) {
        printf("heap neinitializat\n");
        return;
    }

    if(heap.nrElemente == 0) {
        printf("heap gol\n");
        return;
    }

    if(heap.nrElemente == heap.lungime) {
        printf("nu exista elemente ascunse\n");
        return;
    }

    for(int i = heap.nrElemente ; i < heap.lungime ; i++) {
        afisareMasina(heap.masini[i]);
    }
}

// deallocators
void dezalocareHeap(Heap *heap) {
    if(heap == NULL || heap->masini == NULL || heap->nrElemente == 0) return;

    for(int i = 0 ; i < heap->nrElemente ; i++) {
        free(heap->masini[i].model);
        heap->masini[i].model = NULL;
        free(heap->masini[i].numeSofer);
        heap->masini[i].numeSofer = NULL;
    }

    free(heap->masini);
    heap->masini = NULL;

    heap->lungime = 0;
    heap->nrElemente = 0;
}