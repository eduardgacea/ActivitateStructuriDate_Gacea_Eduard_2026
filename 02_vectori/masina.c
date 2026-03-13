#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Masina
{
    int id;
    char *brand;
    float pret;
    char transmisie;
} Masina;

typedef struct VMasina
{
    Masina *masini;
    int nrElemente;
    int capacitate;
} VMasina;

Masina *creazaMasina(int id, const char *brand, float pret, char transmisie);
void afiseazaMasina(Masina *m);
void dezalocareMasina(Masina **m);
VMasina *creazaVectorMasini(int capacitate);
void adaugaMasina(VMasina *v, Masina *m);
void afiseazaMasini(VMasina *v);
void dezalocareVector(VMasina **v);
float calculeazaPretMediu(VMasina *v);
VMasina *copiazaMasiniDupaTransmisie(VMasina *v, char transmisie);

int main()
{
    Masina *m1 = creazaMasina(1, "volkswagen", 7000.0f, 'M');
    Masina *m2 = creazaMasina(2, "bmw", 8499.99, 'A');
    Masina *m3 = creazaMasina(3, "audi", 11500.0f, 'A');
    Masina *m4 = creazaMasina(4, "dacia", 5000.0f, 'M');
    Masina *m5 = creazaMasina(5, "opel", 5500.0f, 'A');
    Masina *m6 = creazaMasina(6, "skoda", 14499.49, 'A');

    VMasina *v = creazaVectorMasini(5);

    float pretMediu;

    afiseazaMasina(m2);

    adaugaMasina(v, m1);
    adaugaMasina(v, m2);
    adaugaMasina(v, m3);
    adaugaMasina(v, m4);
    adaugaMasina(v, m5);
    adaugaMasina(v, m6);

    VMasina *masiniAutomate = copiazaMasiniDupaTransmisie(v, 'A');

    afiseazaMasini(v);

    pretMediu = calculeazaPretMediu(v);
    printf("pretul mediu este: %.2f\n", pretMediu);
    printf("-----------------------------\n");

    dezalocareMasina(&m2);
    afiseazaMasina(m2);

    printf("masini automate: \n");
    afiseazaMasini(masiniAutomate);

    dezalocareMasina(&m1);
    dezalocareMasina(&m3);
    dezalocareMasina(&m4);
    dezalocareMasina(&m5);
    dezalocareMasina(&m6);
    dezalocareVector(&v);
    dezalocareVector(&masiniAutomate);
    afiseazaMasini(v);

    return 0;
}

Masina *creazaMasina(int id, const char *brand, float pret, char transmisie)
{
    Masina *m = malloc(sizeof(Masina));

    if (m)
    {
        m->id = id;
        m->pret = pret;
        m->transmisie = transmisie;

        const char *sursa = brand ? brand : "necunoscut";
        m->brand = malloc(strlen(sursa) + 1);
        if (!m->brand)
        {
            free(m);
            return NULL;
        }
        strcpy_s(m->brand, strlen(sursa) + 1, sursa);
    }

    return m;
}

void afiseazaMasina(Masina *m)
{
    if (!m)
    {
        printf("masina neinitializata\n");
    }
    else
    {
        printf("id: %d\n", m->id);
        printf("brand: %s\n", m->brand ? m->brand : "necunoscut");
        printf("pret: %.2f\n", m->pret);
        printf("transmisie: %c\n", m->transmisie);
    }
    printf("-----------------------------\n");
}

void dezalocareMasina(Masina **m)
{
    if (!m || !*m)
        return;

    free((*m)->brand);
    (*m)->brand = NULL;

    free(*m);
    *m = NULL;
}

VMasina *creazaVectorMasini(int capacitate)
{
    if (capacitate <= 0)
        return NULL;

    VMasina *v = malloc(sizeof(VMasina));

    if (v)
    {
        v->nrElemente = 0;
        v->capacitate = capacitate;
        v->masini = malloc(capacitate * sizeof(Masina));
        if (!v->masini)
        {
            free(v);
            return NULL;
        }
    }

    return v;
}

void adaugaMasina(VMasina *v, Masina *m)
{
    if (!v || !m)
    {
        printf("eroare. masina nu a putut fi adaugata");
        printf("-----------------------------\n");
        return;
    }

    if (v->nrElemente >= v->capacitate)
    {
        printf("vectorul este plin. masina nu a putut fi adaugata\n");
        printf("-----------------------------\n");
        return;
    }

    int i = v->nrElemente;
    v->masini[i].id = m->id;
    v->masini[i].pret = m->pret;
    v->masini[i].transmisie = m->transmisie;
    const char *sursa = m->brand ? m->brand : "necunoscut";
    v->masini[i].brand = malloc(strlen(sursa) + 1);
    if (!v->masini[i].brand)
    {
        printf("eroare alocare brand\n");
        printf("-----------------------------\n");
        return;
    }
    else
    {
        strcpy_s(v->masini[i].brand, strlen(sursa) + 1, sursa);
    }
    v->nrElemente++;

    printf("masina adaugata cu succes\n");
    printf("-----------------------------\n");
}

void afiseazaMasini(VMasina *v)
{
    if (!v || v->nrElemente <= 0)
    {
        printf("vectorul este gol\n");
        printf("-----------------------------\n");
        return;
    }

    for (int i = 0; i < v->nrElemente; i++)
    {
        afiseazaMasina(&(v->masini[i]));
    }
}

void dezalocareVector(VMasina **v)
{
    if (!v || !*v)
        return;

    VMasina *temp = *v;

    for (int i = 0; i < temp->nrElemente; i++)
    {
        free(temp->masini[i].brand);
        temp->masini[i].brand = NULL;
    }

    free(temp->masini);
    temp->masini = NULL;

    free(*v);
    *v = NULL;
}

float calculeazaPretMediu(VMasina *v)
{
    if (!v)
        return 0;
    float sum = 0.0f;

    for (int i = 0; i < v->nrElemente; i++)
    {
        sum += v->masini[i].pret;
    }

    return v->nrElemente == 0 ? 0 : sum / (float)v->nrElemente;
}

VMasina *copiazaMasiniDupaTransmisie(VMasina *v, char transmisie)
{
    if (!v)
        return NULL;
    int j = 0;
    VMasina *c = creazaVectorMasini(v->nrElemente);
    if (!c)
        return NULL;

    for (int i = 0; i < v->nrElemente; i++)
    {
        if (v->masini[i].transmisie == transmisie)
        {
            c->masini[j].id = v->masini[i].id;
            c->masini[j].pret = v->masini[i].pret;
            c->masini[j].transmisie = v->masini[i].transmisie;

            const char *sursa = v->masini[i].brand ? v->masini[i].brand : "necunoscut";
            c->masini[j].brand = malloc(strlen(sursa) + 1);
            if (!c->masini[j].brand)
            {
                printf("eroare. vectorul nu a putut fi copiat");
                printf("-----------------------------\n");
                dezalocareVector(&c);
                return NULL;
            }
            else
            {
                strcpy_s(c->masini[j].brand, strlen(sursa) + 1, sursa);
            }
            j++;
        }
    }
    c->nrElemente = j;

    return c;
}