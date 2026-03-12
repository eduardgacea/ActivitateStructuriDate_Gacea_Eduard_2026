#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Carte
{
    int id;
    char *titlu;
    int nrPagini;
    float pret;
};

typedef struct Carte Carte;

Carte creazaCarte(int id, const char *titlu, int nrPagini, float pret);
void afiseazaCarte(Carte c);
void afiseazaVector(Carte *v, int nrCarti);
void dezalocareCarte(Carte *c);
void dezalocareVector(Carte **v, int nrCarti);

int main()
{
    int nrCarti = 3;

    Carte *carti = malloc(nrCarti * sizeof(Carte));
    if (carti)
    {
        for (int i = 0; i < nrCarti; i++)
        {
            carti[i] = creazaCarte(i + 1, "Carte", (i + 1) * 100, (i + 1) * 20);
        }
    }

    afiseazaVector(carti, nrCarti);
    dezalocareVector(&carti, nrCarti);
    afiseazaVector(carti, nrCarti);

    return 0;
}

Carte creazaCarte(int id, const char *titlu, int nrPagini, float pret)
{
    Carte c;

    c.id = id;
    const char *sursa = titlu ? titlu : "necunoscut";
    c.titlu = malloc(strlen(sursa) + 1);
    if (c.titlu)
        strcpy_s(c.titlu, strlen(sursa) + 1, sursa);
    c.nrPagini = nrPagini;
    c.pret = pret;

    return c;
}

void afiseazaCarte(Carte c)
{
    printf("titlu: %s, nr pagini: %d, pret: %.2f\n", c.titlu, c.nrPagini, c.pret);
}

void afiseazaVector(Carte *v, int nrCarti)
{
    if (!v)
    {
        printf("nu exista carti\n");
    }
    else
    {
        for (int i = 0; i < nrCarti; i++)
        {
            afiseazaCarte(v[i]);
        }
    }
}

void dezalocareCarte(Carte *c)
{
    if (!c)
        return;

    free(c->titlu);
    c->titlu = NULL;
}

void dezalocareVector(Carte **v, int nrCarti)
{
    if (!v || !*v)
        return;

    for (int i = 0; i < nrCarti; i++)
        dezalocareCarte(&(*v)[i]);

    free(*v);
    *v = NULL;
}