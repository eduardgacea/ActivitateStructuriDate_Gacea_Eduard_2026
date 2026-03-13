#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nod Nod;
typedef struct Lista Lista;

struct Nod
{
    int valoare;
    Nod *next;
};

struct Lista
{
    Nod *prim;
};

Lista *creazaLista();
Nod *creazaNod(int valoare);
void adaugaNodLaInceput(Lista *lista, Nod *nod);
void adaugaNodLaFinal(Lista *lista, Nod *nod);
void stergeNodLaInceput(Lista *lista);
void stergeNodLaFinal(Lista *lista);
int calculeazaSuma(Lista *lista);
void afisareLista(Lista *lista);
void dezalocareLista(Lista **lista);

int main()
{
    Lista *lista = creazaLista();
    Nod *nod0 = creazaNod(0);
    Nod *nod1 = creazaNod(1);
    Nod *nod2 = creazaNod(2);
    Nod *nod3 = creazaNod(3);
    Nod *nod4 = creazaNod(4);

    adaugaNodLaFinal(lista, nod1);
    adaugaNodLaFinal(lista, nod2);
    adaugaNodLaFinal(lista, nod3);
    adaugaNodLaFinal(lista, nod4);
    adaugaNodLaInceput(lista, nod0);

    afisareLista(lista);

    printf("suma valorilor din lista este: %d\n", calculeazaSuma(lista));

    stergeNodLaFinal(lista);
    stergeNodLaInceput(lista);
    stergeNodLaInceput(lista);
    stergeNodLaFinal(lista);
    stergeNodLaFinal(lista);
    stergeNodLaFinal(lista);
    stergeNodLaFinal(lista);
    stergeNodLaFinal(lista);
    afisareLista(lista);

    dezalocareLista(&lista);

    afisareLista(lista);

    return 0;
}

Lista *creazaLista()
{
    Lista *lista = malloc(sizeof(Lista));
    if (!lista)
        return NULL;

    lista->prim = NULL;

    return lista;
}

Nod *creazaNod(int valoare)
{
    Nod *nod = malloc(sizeof(Nod));
    if (!nod)
        return NULL;

    nod->valoare = valoare;
    nod->next = NULL;

    return nod;
}

void adaugaNodLaInceput(Lista *lista, Nod *nod)
{
    if (!lista || !nod)
        return;

    if (lista->prim == NULL)
    {
        lista->prim = nod;
        return;
    }

    nod->next = lista->prim;
    lista->prim = nod;
}

void adaugaNodLaFinal(Lista *lista, Nod *nod)
{
    if (!lista || !nod)
        return;

    if (lista->prim == NULL)
    {
        lista->prim = nod;
        return;
    }

    Nod *p = lista->prim;

    while (p->next)
    {
        p = p->next;
    }

    nod->next = NULL;
    p->next = nod;
}

void stergeNodLaInceput(Lista *lista)
{
    if (!lista || !lista->prim)
        return;

    Nod *temp = lista->prim;
    lista->prim = temp->next;
    free(temp);
}

void stergeNodLaFinal(Lista *lista)
{
    if (!lista || !lista->prim)
        return;

    Nod *p = lista->prim;

    if (p->next == NULL)
    {
        free(p);
        lista->prim = NULL;
        return;
    }

    while (p->next->next)
    {
        p = p->next;
    }

    free(p->next);
    p->next = NULL;
}

int calculeazaSuma(Lista *lista)
{
    if (!lista)
        return;

    Nod *p = lista->prim;
    int sum = 0;

    while (p)
    {
        sum += p->valoare;
        p = p->next;
    }

    return sum;
}

void afisareLista(Lista *lista)
{
    if (!lista)
    {
        printf("lista nealocata\n");
        return;
    }

    if (lista->prim == NULL)
    {
        printf("lista este goala\n");
        return;
    }

    Nod *p = lista->prim;

    while (p)
    {
        printf("%d->", p->valoare);
        p = p->next;
    }

    printf("\n");
}

void dezalocareLista(Lista **lista)
{
    if (!lista || !*lista)
        return;

    Nod *p = (*lista)->prim;

    while (p)
    {
        Nod *temp = p->next;
        free(p);
        p = temp;
    }

    free(*lista);
    *lista = NULL;
}