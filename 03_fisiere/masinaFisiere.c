#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

typedef struct StructuraMasina
{
    int id;
    int nrUsi;
    float pret;
    char *model;
    char *numeSofer;
    unsigned char serie;
} Masina;

void afisareMasina(Masina *m);
void afisareVectorMasini(Masina *m, int nrElemente);
Masina *creazaMasinaFisier(FILE *f);
Masina *creazaMasina(int id, int nrUsi, float pret, const char *model, const char *numeSofer, char serie);
Masina *citireVectorMasiniFisier(FILE *f, int *nrMasini);
void dezalocareMasina(Masina **m);
void dezalocareVectorMasini(Masina **m, int *nrElemente);

int main()
{
    int nrMasini;
    FILE *f = fopen("masini.txt", "r");
    if (f == NULL)
    {
        printf("eroare deschidere fisier\n");
        return 1;
    }

    Masina *v = citireVectorMasiniFisier(f, &nrMasini);
    afisareVectorMasini(v, nrMasini);
    dezalocareVectorMasini(&v, &nrMasini);
    afisareVectorMasini(v, nrMasini);

    fclose(f);

    return 0;
}

void afisareMasina(Masina *m)
{
    if (!m)
    {
        printf("eroare. masina nu a putut fi citita\n");
        return;
    }

    printf("masina #%d %s %c cu %d usi, condusa de %s, costa %.2f RON\n", m->id, m->model, m->serie, m->nrUsi, m->numeSofer, m->pret);
}

void afisareVectorMasini(Masina *v, int nrElemente)
{
    if (!v || nrElemente <= 0)
    {
        printf("vectorul este gol\n");
        printf("-----------------------------\n");
        return;
    }

    printf("vectorul de masini:\n\n");
    for (int i = 0; i < nrElemente; i++)
    {
        afisareMasina(&v[i]);
    }

    printf("-----------------------------\n");
}

Masina *creazaMasinaFisier(FILE *f)
{
    if (!f)
    {
        printf("eroare deschidere fisier - masina nu a putut fi creata\n");
        printf("-----------------------------\n");
        return NULL;
    }

    char buffer[BUFFER_SIZE];

    if (fgets(buffer, BUFFER_SIZE, f) == NULL)
    {
        printf("eroare citire fisier - nu mai exista linii ce pot fi citite\n");
        printf("-----------------------------\n");
        return NULL;
    };

    int id = atoi(strtok(buffer, ","));
    int nrUsi = atoi(strtok(NULL, ","));
    float pret = atof(strtok(NULL, ","));
    char *model = strtok(NULL, ",");
    char *numeSofer = strtok(NULL, ",");
    char serie = strtok(NULL, ",")[0];

    return creazaMasina(id, nrUsi, pret, model, numeSofer, serie);
}

Masina *creazaMasina(int id, int nrUsi, float pret, const char *model, const char *numeSofer, char serie)
{
    Masina *m = malloc(sizeof(Masina));
    if (!m)
    {
        printf("eroare. masina nu a putut fi creata\n");
        printf("-----------------------------\n");
        return NULL;
    }

    char *sursa = model ? model : "necunoscut";
    m->model = malloc(strlen(sursa) + 1);
    if (!m->model)
    {
        printf("eroare. masina nu a putut fi creata\n");
        printf("-----------------------------\n");
        free(m);
        return NULL;
    }
    else
    {
        strcpy_s(m->model, strlen(sursa) + 1, sursa);
    }

    sursa = numeSofer ? numeSofer : "necunoscut";
    m->numeSofer = malloc(strlen(sursa) + 1);
    if (!m->numeSofer)
    {
        printf("eroare. masina nu a putut fi creata\n");
        printf("-----------------------------\n");
        free(m->model);
        free(m);
        return NULL;
    }
    else
    {
        strcpy_s(m->numeSofer, strlen(sursa) + 1, sursa);
    }

    m->id = id;
    m->nrUsi = nrUsi;
    m->pret = pret;
    m->serie = serie;

    return m;
}

Masina *citireVectorMasiniFisier(FILE *f, int *nrMasini)
{
    if (!f || !nrMasini)
        return NULL;

    char buffer[BUFFER_SIZE];
    *nrMasini = 0;

    rewind(f);
    while (fgets(buffer, BUFFER_SIZE, f))
    {
        (*nrMasini)++;
    }

    if (*nrMasini == 0)
        return NULL;
    rewind(f);

    Masina *v = malloc(*nrMasini * sizeof(Masina));
    if (!v)
    {
        *nrMasini = 0;
        return NULL;
    }

    for (int i = 0; i < *nrMasini; i++)
    {
        Masina *temp = creazaMasinaFisier(f);
        if (!temp)
        {
            *nrMasini = i;
            break;
        }

        v[i] = *temp;
        free(temp);
    }

    return v;
}

void dezalocareMasina(Masina **m)
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

void dezalocareVectorMasini(Masina **m, int *nrElemente)
{
    if (!m || !*m || !nrElemente)
        return;

    for (int i = 0; i < *nrElemente; i++)
    {
        free((*m)[i].model);
        (*m)[i].model = NULL;

        free((*m)[i].numeSofer);
        (*m)[i].numeSofer = NULL;
    }

    free(*m);
    *m = NULL;
    *nrElemente = 0;
}