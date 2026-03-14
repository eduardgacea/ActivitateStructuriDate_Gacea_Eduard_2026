#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

typedef struct Masina Masina;
typedef struct Nod Nod;
typedef struct Lista Lista;

struct Masina
{
    int id;
    int nrUsi;
    float pret;
    char *model;
    char *numeSofer;
    unsigned char serie;
};

struct Nod
{
    Masina *masina;
    Nod *next;
};

struct Lista
{
    Nod *prim;
};

Masina *creazaMasina(int id, int nrUsi, float pret, const char *model, const char *numeSofer, unsigned char serie);
Nod *creazaNod(Masina *m);
Lista *creazaLista();
Masina *creazaMasinaFisier(FILE *f);
void adaugaNodLaFinal(Lista *l, Nod *n);
Lista *creazaListaFisier(FILE *f);
void afiseazaMasina(Masina *m);
void afiseazaLista(Lista *l);
Masina *getMasinaById(Lista *l, int id);
void adaugaMasinaDupaId(Lista *l, Masina *m, int id);
void stergeNod(Lista *l, Nod **n);
void stergeMasiniDinSeria(Lista *l, unsigned char serie);
void dezalocaMasina(Masina **m);
void dezalocaLista(Lista **l);

int main()
{
    FILE *f = fopen("masini.txt", "r");
    if (!f)
        return 1;

    Lista *lista = creazaListaFisier(f);
    afiseazaLista(lista);

    Masina *m1 = getMasinaById(lista, 1);
    Masina *m10 = getMasinaById(lista, 10);
    Masina *m20 = getMasinaById(lista, 20);
    afiseazaMasina(m1);
    afiseazaMasina(m10);
    afiseazaMasina(m20);

    Masina *m11 = creazaMasina(11, 4, 5500.0f, "punto", "marcel", 'A');
    Masina *m12 = creazaMasina(12, 2, 11499.99, "bmw", "viorel", 'S');
    adaugaMasinaDupaId(lista, m11, 4);
    adaugaMasinaDupaId(lista, m11, 12);
    adaugaMasinaDupaId(lista, m12, 10);
    afiseazaLista(lista);
    stergeMasiniDinSeria(lista, 'A');
    afiseazaLista(lista);

    dezalocaLista(&lista);
    afiseazaLista(lista);

    fclose(f);

    return 0;
}

Masina *creazaMasina(int id, int nrUsi, float pret, const char *model, const char *numeSofer, unsigned char serie)
{
    Masina *m = malloc(sizeof(Masina));
    if (!m)
        return NULL;

    const char *sursaModel = model ? model : "necunoscut";
    m->model = malloc(strlen(sursaModel) + 1);
    if (!m->model)
    {
        free(m);
        return NULL;
    }
    strcpy_s(m->model, strlen(sursaModel) + 1, sursaModel);

    const char *sursaNumeSofer = numeSofer ? numeSofer : "necunoscut";
    m->numeSofer = malloc(strlen(sursaNumeSofer) + 1);
    if (!m->numeSofer)
    {
        free(m->model);
        free(m);
        return NULL;
    }
    strcpy_s(m->numeSofer, strlen(sursaNumeSofer) + 1, sursaNumeSofer);

    m->id = id;
    m->nrUsi = nrUsi;
    m->pret = pret;
    m->serie = serie;

    return m;
}

Nod *creazaNod(Masina *m)
{
    if (!m)
        return NULL;

    Nod *nod = malloc(sizeof(Nod));
    if (!nod)
        return NULL;

    nod->masina = m;
    nod->next = NULL;

    return nod;
}

Lista *creazaLista()
{
    Lista *lista = malloc(sizeof(Lista));
    if (!lista)
        return NULL;

    lista->prim = NULL;

    return lista;
}

Masina *creazaMasinaFisier(FILE *f)
{
    if (!f)
        return NULL;

    char buffer[BUFFER_SIZE];
    int id, nrUsi;
    float pret;
    char *token;
    unsigned char serie;

    char *mData = fgets(buffer, BUFFER_SIZE, f);
    if (!mData)
        return NULL;

    token = strtok(mData, ",");
    id = token ? atoi(token) : 0;
    token = strtok(NULL, ",");
    nrUsi = token ? atoi(token) : 0;
    token = strtok(NULL, ",");
    pret = token ? atof(token) : 0.0f;
    token = strtok(NULL, ",");
    char *model = token ? token : "necunoscut";
    token = strtok(NULL, ",");
    char *numeSofer = token ? token : "necunoscut";
    token = strtok(NULL, ",");
    serie = token ? token[0] : '?';

    return creazaMasina(id, nrUsi, pret, model, numeSofer, serie);
}

void adaugaNodLaFinal(Lista *l, Nod *n)
{
    if (!l || !n)
        return;

    if (l->prim == NULL)
    {
        l->prim = n;
        return;
    }

    Nod *p = l->prim;
    while (p->next)
    {
        p = p->next;
    }

    p->next = n;
    n->next = NULL;
}

Lista *creazaListaFisier(FILE *f)
{
    if (!f)
        return NULL;

    Lista *l = creazaLista();
    if (!l)
        return NULL;

    Masina *m;

    while ((m = creazaMasinaFisier(f)) != NULL)
    {
        Nod *n = creazaNod(m);
        adaugaNodLaFinal(l, n);
    }

    return l;
}

void afiseazaMasina(Masina *m)
{
    if (!m)
    {
        printf("masina neinitializata\n");
        return;
    }

    printf("masina #%d %s %c cu %d usi, condusa de %s, costa %.2f RON\n", m->id, m->model ? m->model : "necunoscut", m->serie, m->nrUsi, m->numeSofer ? m->numeSofer : "necunoscut", m->pret);
}

void afiseazaLista(Lista *l)
{
    if (!l)
    {
        printf("lista neinitializata\n");
        return;
    }

    Nod *p = l->prim;
    while (p)
    {
        afiseazaMasina(p->masina);
        p = p->next;
    }
}

Masina *getMasinaById(Lista *l, int id)
{
    if (!l)
        return NULL;

    Nod *p = l->prim;
    while (p)
    {
        if (p->masina->id == id)
            return p->masina;
        p = p->next;
    }

    return NULL;
}

void adaugaMasinaDupaId(Lista *l, Masina *m, int id)
{
    if (!l || !m)
        return;
    Nod *n = creazaNod(m);
    if (!n)
        return;

    Nod *p = l->prim;
    while (p)
    {
        if (p->masina->id == id)
        {
            n->next = p->next;
            p->next = n;
            return;
        }
        p = p->next;
    }
}

void stergeNod(Lista *l, Nod **n)
{
    if (!l || !n || !*n)
        return;

    Nod *target = *n;

    if (l->prim == target)
    {
        l->prim = target->next;
    }
    else
    {
        Nod *p = l->prim;

        while (p && p->next != target)
            p = p->next;
        // daca am parcursa toata lista si p == NULL, inseamna ca nu am gasit target
        if (!p)
            return;

        p->next = target->next;
    }

    dezalocaMasina(&target->masina);
    free(target);
    *n = NULL;
}

void stergeMasiniDinSeria(Lista *l, unsigned char serie)
{
    if (!l)
        return;

    Nod *p = l->prim;
    while (p)
    {
        Nod *temp = p->next;
        if (p->masina->serie == serie)
            stergeNod(l, &p);

        p = temp;
    }
}

void dezalocaMasina(Masina **m)
{
    if (!m || !*m)
        return;

    free((*m)->model);
    (*m)->model = NULL;

    free((*m)->numeSofer);
    (*m)->numeSofer = NULL;

    free(*m);
    *m = NULL;
}

void dezalocaLista(Lista **l)
{
    if (!l || !*l)
        return;

    Nod *p = (*l)->prim;

    while (p)
    {
        Nod *temp = p->next;
        dezalocaMasina(&p->masina);
        free(p);

        p = temp;
    }

    free(*l);
    *l = NULL;
}