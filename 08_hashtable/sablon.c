#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student Student;
typedef struct Nod Nod;
typedef struct Hashtable Hashtable;

struct Student {
    char* nume;
    int varsta;
    float medie;
};

struct Nod {
    Student student;
    Nod* next;
};

struct Hashtable {
    int dim;
    Nod** vector;
};

Student citireStudentDinFisier(FILE* file) {
    Student student = {0};
    if(file == NULL) return student;

    char buffer[100];
    char sep[3] = ",\n";
    char* token;

    fgets(buffer, 100, file);

    token = strtok(buffer, sep);
    const char* src = token == NULL ? "necunoscut" : token;
    student.nume = malloc(strlen(src) + 1);
    if(student.nume == NULL) return student;
    strcpy(student.nume, src);

    token = strtok(NULL, sep);
    student.varsta = atoi(token);

    token = strtok(NULL, sep);
    student.medie = atof(token);

    return student;
}

void afisareStudent(Student s) {
    if(s.nume == NULL) {
        printf("nume: necunoscut, varsta: %d, medie: %.2f\n", s.varsta, s.medie);
    } else {
        printf("nume: %s, varsta: %d, medie: %.2f\n", s.nume, s.varsta, s.medie);
    }
}

Hashtable initializareHashtable(int dim) {
    Hashtable ht = {0};
    if(dim <= 0) return ht;

    ht.dim = dim;

    ht.vector = malloc(dim * sizeof(Nod *));
    if(ht.vector == NULL) return ht;

    for(int i = 0 ; i < dim ; i++) {
        ht.vector[i] = NULL;
    }

    return ht;
}

int hash(int dim, int varsta) {
    return varsta % dim;
}

void inserare(Hashtable ht, Student s) {
    if(ht.dim > 0) {
        Nod* nod = malloc(sizeof(Nod));
        if(nod == NULL) return;
        nod->next = NULL;
        nod->student = s;

        int poz = hash(ht.dim, s.varsta);
        Nod* aux = ht.vector[poz];
        
        if(aux != NULL) {
            while(aux->next) {
                aux = aux->next;
            }
            aux->next = nod;
        } else {
            ht.vector[poz] = nod;
        }
    }
}

Hashtable citireHashtableDinFisier(const char* numeFisier, int dim) {
    Hashtable ht = initializareHashtable(dim);
    if(numeFisier == NULL) return ht;

    FILE* file = fopen(numeFisier, "r");
    if(file == NULL) return ht;

    while(!feof(file)) {
        inserare(ht, citireStudentDinFisier(file));
    }

    fclose(file);
    
    return ht;
}

void afisareHashtable(Hashtable ht) {
    if(ht.dim <= 0) {
        printf("--------------------------\n");
        printf("hashtable gol\n");
        printf("--------------------------\n");
        return;
    }

    if(ht.vector == NULL) {
        printf("--------------------------\n");
        printf("hashtable neinitializat\n");
        printf("--------------------------\n");
        return;
    }

    printf("--------------------------\n");
    for(int i = 0 ; i < ht.dim ; i++) {
        printf("=== LISTA %d===\n", i);
        Nod *cap = ht.vector[i];
        while(cap) {
            afisareStudent(cap->student);
            cap = cap->next;
        }
    }
    printf("--------------------------\n");
}

Student getStudentDupaVarsta(Hashtable ht, int varsta) {
    Student s = {0};
    int poz = hash(ht.dim, varsta);
    if(0 <= poz && poz < ht.dim) {
        Nod *cap = ht.vector[poz];
        if(cap == NULL) return s;

        while(cap) {
            if(cap->student.varsta == varsta) {
                 s = cap->student;
                 return s;
            }
            cap = cap->next;
        }
    }

    return s;
}

void dezalocareHashtable(Hashtable *ht) {
    if(ht == NULL) return;

    for(int i = 0 ; i < ht->dim ; i++) {
        Nod* cap = ht->vector[i];
        while(cap) {
            Nod* aux = cap->next;
            free(cap->student.nume);
            free(cap);
            cap = aux;
        }
    }
    free(ht->vector);
    ht->vector = NULL;
}

int main() {
    Hashtable ht = citireHashtableDinFisier("studenti.txt", 4);

    afisareHashtable(ht);

    Student s1 = getStudentDupaVarsta(ht, 29);
    Student s2 = getStudentDupaVarsta(ht, 40);
    afisareStudent(s1);
    afisareStudent(s2);
    
    dezalocareHashtable(&ht);
    afisareHashtable(ht);

    return 0;
}