// exercitiu pentru lucrul cu arrays de string-uri in C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Catalog
{
    char *nume;
    int nrMaterii;
    char **materii;
    float *note;
};

typedef struct Catalog Catalog;

Catalog *creazaCatalog(const char *nume, int nrMaterii, const char **materii, float *note);
void afiseazaCatalog(const Catalog *c);
void dezalocareCatalog(Catalog **c);
void modificareNota(Catalog *c, const char *materie, float nota);

int main()
{
    const char *materii[] = {"POO", "SDD", "BD"};
    const float note[3] = {6, 10, 4};
    Catalog *c = creazaCatalog("anul 2", 3, materii, note);

    afiseazaCatalog(c);
    modificareNota(c, "BD", 5);
    afiseazaCatalog(c);
    dezalocareCatalog(&c);
    afiseazaCatalog(c);

    return 0;
}

Catalog *creazaCatalog(const char *nume, int nrMaterii, const char **materii, float *note)
{
    Catalog *catalog = malloc(sizeof(Catalog));

    if (catalog)
    {
        // nume
        char *sursa = nume ? nume : "necunoscut";
        catalog->nume = malloc(strlen(sursa) + 1);
        if (catalog->nume)
            strcpy_s(catalog->nume, strlen(sursa) + 1, sursa);

        // nr materii
        catalog->nrMaterii = nrMaterii;

        // materii
        catalog->materii = malloc(catalog->nrMaterii * sizeof(char *));
        if (catalog->materii)
        {
            for (int i = 0; i < catalog->nrMaterii; i++)
            {
                catalog->materii[i] = malloc(strlen(materii[i]) + 1);
                if (catalog->materii[i])
                    strcpy_s(catalog->materii[i], strlen(materii[i]) + 1, materii[i]);
            }
        }

        // note
        catalog->note = malloc(catalog->nrMaterii * sizeof(float));
        if (catalog->note)
        {
            for (int i = 0; i < catalog->nrMaterii; i++)
            {
                catalog->note[i] = note[i];
            }
        }
    }

    return catalog;
}

void afiseazaCatalog(const Catalog *c)
{
    if (c)
    {
        printf("catalog: %s\n", c->nume);
        if (c->nrMaterii == 0)
        {
            printf("fara materii adaugate\n");
        }
        else
        {
            for (int i = 0; i < c->nrMaterii; i++)
            {
                printf("%s: %.2f\n", c->materii[i], c->note[i]);
            }
        }
    }
    else
    {
        printf("catalogul nu a fost creat\n");
    }
}

void dezalocareCatalog(Catalog **c)
{
    if (!c || !*c)
        return;

    Catalog *temp = *c;

    // dezalocare nume
    free(temp->nume);
    temp->nume = NULL;

    // dezalocare materii
    for (int i = 0; i < temp->nrMaterii; i++)
    {
        free(temp->materii[i]);
        temp->materii[i] = NULL;
    }
    free(temp->materii);
    temp->materii = NULL;

    // dezalocare note
    free(temp->note);
    temp->note = NULL;

    *c = NULL;
}

void modificareNota(Catalog *catalog, const char *materie, float nota)
{
    if (!catalog || !materie)
        return;

    for (int i = 0; i < catalog->nrMaterii; i++)
    {
        if (strcmp(catalog->materii[i], materie) == 0)
        {
            catalog->note[i] = nota;
            break;
        };
    }
}