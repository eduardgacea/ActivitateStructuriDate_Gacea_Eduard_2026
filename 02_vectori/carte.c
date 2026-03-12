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
Carte *getCarteById(Carte *v, int nrCarti, int id);
// o carte este considerata scumpa daca pretul ei depaseste valoarea variabilei prag
Carte *getPrimaCarteScumpa(Carte *v, int nrCarti, float prag);
Carte *copiazaPrimeleNCarti(Carte *v, int nrCarti, int nrElemente);
void copiazaCartiScumpe(Carte *v, int nrCarti, float prag, Carte **u, int *nrCartiScumpe);

int main()
{
    int nrCarti = 5;
    int idTarget = 3;
    float prag = 70.0;
    int nrCartiCopiate = 2;
    int nrCartiScumpe = 0;

    Carte *carti = malloc(nrCarti * sizeof(Carte));
    if (carti)
    {
        for (int i = 0; i < nrCarti; i++)
        {
            carti[i] = creazaCarte(i + 1, "Carte", (i + 1) * 100, (i + 1) * 20);
        }
    }

    Carte *cartiCopiate = copiazaPrimeleNCarti(carti, nrCarti, nrCartiCopiate);
    Carte *cartiScumpe = malloc(nrCarti * sizeof(Carte));
    if (cartiScumpe)
    {
        copiazaCartiScumpe(carti, nrCarti, prag, &cartiScumpe, &nrCartiScumpe);
    }

    Carte *carte2 = getCarteById(carti, nrCarti, idTarget);
    if (carte2)
    {
        printf("cartea cu id = %d: ", idTarget);
        afiseazaCarte(*carte2);
    }
    else
    {
        printf("cartea cu id = %d nu exista\n", idTarget);
    }

    Carte *carteScumpa = getPrimaCarteScumpa(carti, nrCarti, prag);
    if (carteScumpa)
    {
        printf("prima carte scumpa este: ");
        afiseazaCarte(*carteScumpa);
    }
    else
    {
        printf("nu exista carti scumpe\n");
    }

    printf("carti originale:\n");
    afiseazaVector(carti, nrCarti);

    printf("carti copiate:\n");
    afiseazaVector(cartiCopiate, nrCartiCopiate);

    printf("carti scumpe:\n");
    afiseazaVector(cartiScumpe, nrCartiScumpe);

    dezalocareVector(&carti, nrCarti);
    afiseazaVector(carti, nrCarti);

    dezalocareVector(&cartiCopiate, nrCartiCopiate);
    dezalocareVector(&cartiScumpe, nrCartiScumpe);
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

Carte *getCarteById(Carte *v, int nrCarti, int id)
{
    for (int i = 0; i < nrCarti; i++)
    {
        if (v[i].id == id)
        {
            return &v[i];
        }
    }

    return NULL;
}

Carte *getPrimaCarteScumpa(Carte *v, int nrCarti, float prag)
{
    for (int i = 0; i < nrCarti; i++)
    {
        if (v[i].pret >= prag)
            return &v[i];
    }

    return NULL;
}

Carte *copiazaPrimeleNCarti(Carte *v, int nrCarti, int nrElemente)
{
    if (nrElemente <= 0 || nrCarti < nrElemente)
        return NULL;
    Carte *u = malloc(nrElemente * sizeof(Carte));

    if (u)
    {
        for (int i = 0; i < nrElemente; i++)
        {
            u[i].id = v[i].id;
            u[i].nrPagini = v[i].nrPagini;
            u[i].pret = v[i].pret;
            u[i].titlu = malloc(strlen(v[i].titlu) + 1);
            if (u[i].titlu)
            {
                strcpy_s(u[i].titlu, strlen(v[i].titlu) + 1, v[i].titlu);
            }
        }
    }

    return u;
}

void copiazaCartiScumpe(Carte *v, int nrCarti, float prag, Carte **u, int *nrCartiScumpe)
{
    if (!v || !u || !*u || !nrCartiScumpe)
        return;

    int j = 0;
    for (int i = 0; i < nrCarti; i++)
    {
        if (v[i].pret >= prag)
        {
            (*u)[j].id = v[i].id;
            (*u)[j].nrPagini = v[i].nrPagini;
            (*u)[j].pret = v[i].pret;
            (*u)[j].titlu = malloc(strlen(v[i].titlu) + 1);
            if ((*u)[j].titlu)
            {
                strcpy_s((*u)[j].titlu, strlen(v[i].titlu) + 1, v[i].titlu);
            }
            j++;
        }
    }

    *nrCartiScumpe = j;
}