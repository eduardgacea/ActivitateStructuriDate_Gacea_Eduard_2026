#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nod Nod;
typedef struct Lista Lista;

struct Nod
{
    int valoare;
    Nod *prev;
    Nod *next;
};

struct Lista
{
    Nod *prim;
    Nod *ultim;
};

Lista *creazaLista();
Nod *creazaNod(int valoare);
void adaugaLaInceput(Lista *lista, Nod *nod);
void adaugaLaSfarsit(Lista *lista, Nod *nod);
void adaugaDupaReferinta(Lista *lista, Nod *nod, int valoare);
int valoareMaxima(Lista *l);
void afisareLista(Lista *lista);
void stergeLista(Lista **lista);
void stergeNod(Nod **nod);

int main()
{
    Lista *lista = creazaLista();
    Nod *nod3 = creazaNod(3);
    adaugaLaSfarsit(lista, creazaNod(1));
    adaugaLaSfarsit(lista, creazaNod(2));
    adaugaLaSfarsit(lista, nod3);
    adaugaLaSfarsit(lista, creazaNod(5));
    adaugaLaSfarsit(lista, creazaNod(6));
    adaugaLaInceput(lista, creazaNod(0));
    afisareLista(lista);

    adaugaDupaReferinta(lista, nod3, 4);
    afisareLista(lista);

    int max = valoareMaxima(lista);
    printf("valoarea maxima: %d\n", max);

    adaugaLaInceput(lista, creazaNod(7));
    max = valoareMaxima(lista);
    printf("valoarea maxima: %d\n", max);

    Nod *nod8 = creazaNod(8);
    adaugaLaSfarsit(lista, nod8);
    max = valoareMaxima(lista);
    printf("valoarea maxima: %d\n", max);

    adaugaDupaReferinta(lista, nod8, 9);
    afisareLista(lista);

    stergeLista(&lista);
    afisareLista(lista);

    return 0;
}

Lista *creazaLista()
{
    Lista *lista = malloc(sizeof(Lista));
    if (!lista)
        return NULL;

    lista->prim = NULL;
    lista->ultim = NULL;

    return lista;
}

Nod *creazaNod(int valoare)
{
    Nod *nod = malloc(sizeof(Nod));
    if (!nod)
        return NULL;

    nod->valoare = valoare;
    nod->prev = NULL;
    nod->next = NULL;

    return nod;
}

void adaugaLaInceput(Lista *lista, Nod *nod)
{
    if (!lista || !nod)
        return;

    if (lista->prim == NULL)
    {
        lista->prim = lista->ultim = nod;
        nod->prev = nod->next = NULL;
        return;
    }

    nod->next = lista->prim;
    nod->prev = NULL;
    lista->prim->prev = nod;
    lista->prim = nod;
}

void adaugaLaSfarsit(Lista *lista, Nod *nod)
{
    if (!lista || !nod)
        return;

    if (lista->ultim == NULL)
    {
        lista->prim = lista->ultim = nod;
        nod->prev = nod->next = NULL;
        return;
    }

    nod->prev = lista->ultim;
    nod->next = NULL;
    lista->ultim->next = nod;
    lista->ultim = nod;
}

void adaugaDupaReferinta(Lista *lista, Nod *nod, int valoare)
{
    if (!lista || !nod)
        return;
    Nod *nodNou = creazaNod(valoare);
    if (!nodNou)
        return;

    if (nod->next == NULL)
    {
        adaugaLaSfarsit(lista, nodNou);
        return;
    }

    nodNou->next = nod->next;
    nodNou->prev = nod;
    nod->next->prev = nodNou;
    nod->next = nodNou;
}

int valoareMaxima(Lista *lista)
{
    if (!lista || !lista->prim)
        return NULL;

    int max = lista->prim->valoare;

    Nod *p = lista->prim->next;
    while (p)
    {
        if (p->valoare > max)
            max = p->valoare;
        p = p->next;
    }

    return max;
}

void afisareLista(Lista *lista)
{
    if (!lista)
    {
        printf("lista neinitializata\n");
        return;
    }

    Nod *p = lista->prim;
    while (p)
    {
        printf("%d <=> ", p->valoare);
        p = p->next;
    }
    printf("\n");
}

void stergeLista(Lista **lista)
{
    if (!lista || !*lista)
        return;

    Nod *p = (*lista)->prim;
    while (p)
    {
        Nod *temp = p->next;
        stergeNod(&p);
        p = temp;
    }

    free(*lista);
    *lista = NULL;
}

void stergeNod(Nod **nod)
{
    if (!nod || !*nod)
        return;

    (*nod)->prev = NULL;
    (*nod)->next = NULL;
    free(*nod);
    *nod = NULL;
}