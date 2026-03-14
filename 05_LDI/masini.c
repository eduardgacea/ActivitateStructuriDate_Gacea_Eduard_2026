#define _CRT_SECURE_NO_WARNINGS

#define BUFFER_SIZE 128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Lista Lista;
typedef struct Nod Nod;
typedef struct Masina Masina;

struct Lista
{
    Nod *prim;
    Nod *ultim;
};

struct Nod
{
    Masina *masina;
    Nod *prev;
    Nod *next;
};

struct Masina
{
    int id;
    int nrUsi;
    float pret;
    char *model;
    char *numeSofer;
    unsigned char serie;
};

Masina *creazaMasina(int id, int nrUsi, float pret, const char *model, const char *numeSofer, unsigned char serie);
Nod *creazaNod(Masina *m);
Lista *creazaLista();
Masina *citesteMasinaFisier(FILE *f);
Lista *citesteListaFisier(const char *numeFisier);
void adaugaNodLaSfarsit(Lista *lista, Nod *nod);
void afiseazaMasina(Masina *masina);
void afiseazaLista(Lista *lista);
void stergeMasinaDupaId(Lista *lista, int id);
void stergeMasina(Masina **masina);
void stergeLista(Lista **lista);

int main()
{
    Lista *lista = citesteListaFisier("masini.txt");
    afiseazaLista(lista);

    stergeLista(&lista);
    afiseazaLista(lista);

    return 0;
}

Masina *creazaMasina(int id, int nrUsi, float pret, const char *model, const char *numeSofer, unsigned char serie)
{
    Masina *masina = malloc(sizeof(Masina));
    if (!masina)
        return NULL;

    const char *sursaModel = model ? model : "necunoscut";
    masina->model = malloc(strlen(sursaModel) + 1);
    if (!masina->model)
    {
        free(masina);
        return NULL;
    }
    strcpy_s(masina->model, strlen(sursaModel) + 1, sursaModel);

    const char *sursaNumeSofer = numeSofer ? numeSofer : "necunoscut";
    masina->numeSofer = malloc(strlen(sursaNumeSofer) + 1);
    if (!masina->numeSofer)
    {
        free(masina->model);
        free(masina);
        return NULL;
    }
    strcpy_s(masina->numeSofer, strlen(sursaNumeSofer) + 1, sursaNumeSofer);

    masina->id = id;
    masina->nrUsi = nrUsi;
    masina->pret = pret;
    masina->serie = serie;

    return masina;
}

Nod *creazaNod(Masina *masina)
{
    if (!masina)
        return NULL;
    Nod *nod = malloc(sizeof(Nod));
    if (!nod)
        return NULL;

    nod->masina = masina;
    nod->prev = NULL;
    nod->next = NULL;

    return nod;
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

Masina *citesteMasinaFisier(FILE *f)
{
    if (!f)
        return NULL;

    char buffer[BUFFER_SIZE];
    int id, nrUsi;
    float pret;
    char *token, *data, *model, *numeSofer;
    unsigned char serie;

    data = fgets(buffer, BUFFER_SIZE, f);
    if (!data)
        return NULL;

    token = strtok(data, ",");
    id = token ? atoi(token) : 0;

    token = strtok(NULL, ",");
    nrUsi = token ? atoi(token) : 0;

    token = strtok(NULL, ",");
    pret = token ? atof(token) : 0;

    token = strtok(NULL, ",");
    model = token ? token : "necunoscut";

    token = strtok(NULL, ",");
    numeSofer = token ? token : "necunoscut";

    token = strtok(NULL, ",");
    serie = token ? token[0] : '?';

    return creazaMasina(id, nrUsi, pret, model, numeSofer, serie);
}

Lista *citesteListaFisier(const char *numeFisier)
{
    if (!numeFisier)
        return NULL;

    FILE *f = fopen(numeFisier, "r");
    if (!f)
        return NULL;

    Lista *lista = creazaLista();
    if (!lista)
    {
        fclose(f);
        return NULL;
    }

    Masina *masina;

    while ((masina = citesteMasinaFisier(f)) != NULL)
    {
        Nod *nod = creazaNod(masina);
        if (!nod)
        {
            stergeMasina(&masina);
            continue;
        }
        adaugaNodLaSfarsit(lista, nod);
    }

    fclose(f);

    return lista;
}

void adaugaNodLaSfarsit(Lista *lista, Nod *nod)
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

void afiseazaMasina(Masina *masina)
{
    if (!masina)
    {
        printf("masina neinitializata\n");
        return;
    }

    printf("masina #%d %s %c cu %d usi, condusa de %s, costa %.2f RON\n", masina->id, masina->model, masina->serie, masina->nrUsi, masina->numeSofer, masina->pret);
}

void afiseazaLista(Lista *lista)
{
    if (!lista)
    {
        printf("lista neinitializata\n");
        return;
    }

    if (lista->prim == NULL)
    {
        printf("lista goala\n");
        return;
    }

    Nod *p = lista->prim;

    while (p)
    {
        afiseazaMasina(p->masina);
        p = p->next;
    }
}

void stergeMasinaDupaId(Lista *lista, int id)
{
    if (!lista || !lista->prim)
        return;

    Nod *p = lista->prim;

    if (p->masina->id == id)
    {
        // to do
    }
}

void stergeMasina(Masina **masina)
{
    if (!masina || !*masina)
        return;

    free((*masina)->model);
    (*masina)->model = NULL;

    free((*masina)->numeSofer);
    (*masina)->numeSofer = NULL;

    free(*masina);
    *masina = NULL;
}

void stergeLista(Lista **lista)
{
    if (!lista || !*lista)
        return;

    if ((*lista)->prim == NULL)
    {
        free(*lista);
        *lista = NULL;
        return;
    }

    Nod *p = (*lista)->prim;
    while (p)
    {
        Nod *temp = p->next;
        stergeMasina(&p->masina);
        free(p);

        p = temp;
    }

    free(*lista);
    *lista = NULL;
}