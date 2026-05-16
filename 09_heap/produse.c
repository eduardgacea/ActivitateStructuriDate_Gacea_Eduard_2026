#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100

typedef struct Produs Produs;
typedef struct Heap Heap;

struct Produs {
    char* nume;
    int cant;
    float pret;
};

struct Heap {
    Produs* produse;
    int lungime;
    int nrProduse;
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

Heap initializareHeap(int lungime) {
    Heap heap = {0};
    if(lungime <= 0) return heap;

    heap.produse = malloc(lungime * sizeof(Produs));
    if(heap.produse == NULL) return heap;

    heap.lungime = lungime;
    heap.nrProduse = 0;

    return heap;
}

void filtreazaHeap(Heap heap, int poz) {
    if(heap.produse == NULL) return;
    if(poz < 0 || poz >= heap.lungime) return;

    int pozCopilStanga = 2 * poz + 1;
    int pozCopilDreapta = 2 * poz + 2;
    int pozMinim = poz;

    if(pozCopilStanga < heap.nrProduse && heap.produse[pozCopilStanga].cant < heap.produse[pozMinim].cant) {
        pozMinim = pozCopilStanga;
    }

    if(pozCopilDreapta < heap.nrProduse && heap.produse[pozCopilDreapta].cant < heap.produse[pozMinim].cant) {
        pozMinim = pozCopilDreapta;
    }

    if(pozMinim != poz) {
        Produs aux = heap.produse[poz];
        heap.produse[poz] = heap.produse[pozMinim];
        heap.produse[pozMinim] = aux;
        if(pozMinim < (heap.nrProduse - 2) / 2) {
            filtreazaHeap(heap, pozMinim);
        }
    }
}

Heap citireHeapDinFisier(const char* numeFisier, int lungime) {
    Heap heap = {0};
    if(numeFisier == NULL || lungime <= 0) return heap;

    FILE* f = fopen(numeFisier, "r");
    if(f == NULL) return heap;

    heap = initializareHeap(lungime);
    if(heap.produse == NULL) return heap;

    while(!feof(f)) {
        heap.produse[heap.nrProduse++] = citireProdusDinFisier(f);
        if(heap.nrProduse >= heap.lungime) break;
    }

    for(int i = (heap.nrProduse - 2) / 2 ; i >= 0 ; i--) {
        filtreazaHeap(heap, i);
    }

    return heap;
}

void afisareProdus(Produs p) {
    if(p.nume == NULL) {
        printf("nume: necunoscut, cantitate: %d, pret: %.2f\n", p.cant, p.pret);
    } else {
        printf("nume: %s, cantitate: %d, pret: %.2f\n", p.nume, p.cant, p.pret);
    }
}

void afisareHeap(Heap heap) {
    if(heap.produse == NULL || heap.lungime == 0) {
        printf("heap neinitializat\n");
        return;
    }

    if(heap.nrProduse == 0) {
        printf("heap gol\n");
        return;
    }

    for(int i = 0 ; i < heap.nrProduse ; i++) {
        afisareProdus(heap.produse[i]);
    }
}

void dealocareHeap(Heap* heap) {
    if(heap->produse == NULL || heap->lungime <= 0) return;
    
    for(int i = 0 ; i < heap->nrProduse ; i++) {
        free(heap->produse[i].nume);
        heap->produse[i].nume = NULL;
        heap->produse[i].cant = 0;
        heap->produse[i].pret = 0.0f;
    }

    free(heap->produse);
    heap->produse = NULL;
    heap->lungime = heap->nrProduse = 0;
}

Produs extrageElementMinim(Heap* heap) {
    Produs p = {0};

    if(heap->nrProduse > 0) {
        p = heap->produse[0];
        heap->produse[0] = heap->produse[heap->nrProduse - 1];
        heap->produse[heap->nrProduse - 1] = p;
        heap->nrProduse--;

        for(int i = (heap->nrProduse - 2) / 2 ; i >= 0 ; i--) {
            filtreazaHeap(*heap, i);
        }
    }

    return p;
}

int main() {
    Heap heap = citireHeapDinFisier("produse.txt", 20);
    Produs min = extrageElementMinim(&heap);

    afisareHeap(heap);
    printf("produsul minim este: ");
    afisareProdus(min);

    dealocareHeap(&heap);

    afisareHeap(heap);

    return 0;
}