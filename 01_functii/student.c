#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student
{
    char *nume;
    int an;
    float medie;
    char initialaParinte;
};

typedef struct Student Student;

Student *creareStudent(char *nume, int an, float medie, char initialaParinte);
void afisareStudent(Student *s);
void dezalocareStudent(Student *s);
void modificaAn(Student *s, int an);
void modificaNume(Student *s, const char *nume);

int main()
{
    Student *s = creareStudent("ionescu", 2, 8.48, 'G');
    afisareStudent(s);
    modificaAn(s, 3);
    modificaNume(s, "popescu");
    afisareStudent(s);
    afisareStudent(NULL);

    dezalocareStudent(s);
    s = NULL;
    afisareStudent(s);

    return 0;
}

Student *creareStudent(const char *nume, int an, float medie, char initialaParinte)
{
    Student *s = malloc(sizeof(Student));

    if (!s)
        return NULL;

    char *sursa = nume ? nume : "necunoscut";
    s->nume = malloc(strlen(sursa) + 1);
    if (s->nume)
    {
        strcpy_s(s->nume, strlen(sursa) + 1, sursa);
    }

    s->an = an;
    s->medie = medie;
    s->initialaParinte = initialaParinte;
    return s;
}

void afisareStudent(Student *s)
{
    if (s)
    {
        printf("nume: %s\n", s->nume ? s->nume : "necunoscut");
        printf("an: %d\n", s->an);
        printf("medie: %.2f\n", s->medie);
        printf("initiala parinte: %c\n", s->initialaParinte);
    }
    else
    {
        printf("studentul nu a fost initializat corect\n");
    }
    printf("------------------\n\n");
}

void dezalocareStudent(Student *s)
{
    if (s)
    {
        free(s->nume);
        free(s);
    }
}

void modificaAn(Student *s, int an)
{
    if (!s)
        return;
    s->an = an;
}

void modificaNume(Student *s, const char *nume)
{
    if (!s)
        return;

    int len = strlen(nume) + 1;
    char *numeNou = nume ? nume : "necunoscut";

    free(s->nume);

    s->nume = malloc(len);
    if (s->nume)
    {
        strcpy_s(s->nume, len, numeNou);
    }
}