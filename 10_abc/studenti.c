#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student Student;
typedef struct Nod Nod;

struct Student {
    char* nume;
    int varsta;
    float medie;
};

struct Nod {
    Student student;
    Nod* st;
    Nod* dr;
};

Student citireStudentDinFisier(FILE* file) {
    Student s = {0};
    if(file == NULL) return s;

    char buffer[100];
    char sep[3] = ",\n";
    char* token;

    fgets(buffer, 100, file);

    token = strtok(buffer, sep);
    char* src = token ? token : "necunoscut";
    s.nume = malloc(strlen(src) + 1);
    if(s.nume == NULL) return s;
    strcpy(s.nume, src);

    token = strtok(NULL, sep);
    s.varsta = atoi(token);

    token = strtok(NULL, sep);
    s.medie = atof(token);

    return s;
}

Nod* initializareNod(Student s) {
    Nod* nod = malloc(sizeof(Nod));
    if(nod == NULL) return NULL;

    nod->student = s;
    nod->st = nod->dr = NULL;

    return nod;
}

void inserare(Nod** rad, Student s) {
    if(*rad == NULL) {
        Nod* nod = initializareNod(s);
        *rad = nod;
    } else {
        if((*rad)->student.medie < s.medie) {
            inserare(&(*rad)->dr, s);
        }

        if((*rad)->student.medie > s.medie) {
            inserare(&(*rad)->st, s);
        }
    }
}

Nod* initializareArboreDinFisier(const char* numeFisier) {
    if(numeFisier == NULL) return NULL;
    FILE* file = fopen(numeFisier, "r");
    if(file == NULL) return NULL;

    Nod* arbore = NULL;
    
    while(!feof(file)) {
        inserare(&arbore, citireStudentDinFisier(file));
    }

    fclose(file);

    return arbore;
}

void afisareStudent(Student s) {
    if(s.nume == NULL) {
        printf("nume: necunoscut, varsta: %d, medie: %.2f\n", s.varsta, s.medie);
    } else {
        printf("nume: %s, varsta: %d, medie: %.2f\n", s.nume, s.varsta, s.medie);
    }
}

void afisareArbore(Nod* rad) {
    if(rad) {
        afisareArbore(rad->st);
        afisareStudent(rad->student);
        afisareArbore(rad->dr);
    }
}

void dezalocareArbore(Nod** rad) {
    if(*rad) {
        dezalocareArbore(&(*rad)->st);
        dezalocareArbore(&(*rad)->dr);

        free((*rad)->student.nume);

        free(*rad);
        *rad = NULL;
    }
}

Student getStudentByMedie(Nod* rad, float medie) {
    Student s = {0};

    if(rad) {
        if(rad->student.medie == medie) {
            s = rad->student;
            const char* src = rad->student.nume ? rad->student.nume : "necunoscut";
            s.nume = malloc(strlen(src) + 1);
            if(s.nume == NULL) return s;
            strcpy(s.nume, src);
            return s;
        }

        if(rad->student.medie < medie) {
            s = getStudentByMedie(rad->dr, medie);
        }

        if(rad->student.medie > medie) {
            s = getStudentByMedie(rad->st, medie);
        }
    }

    return s;
}

int determinaNumarNoduri(Nod* rad) {
	if (rad) {
		return determinaNumarNoduri(rad->st) + determinaNumarNoduri(rad->dr) + 1;
	}
	return 0;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int getInaltimeArbore(Nod* rad) {
    if(rad) {
        return 1 + max(getInaltimeArbore(rad->st), getInaltimeArbore(rad->dr));
    }

    return 0;
}

float getSumaNote(Nod* rad) {
    if(rad) {
        return getSumaNote(rad->st) + getSumaNote(rad->dr) + rad->student.medie;
    }

    return 0.0f;
}

float calculeazaMedieGenerala(Nod* rad) {
    return getSumaNote(rad) / determinaNumarNoduri(rad);
}

int main() {
    Nod* arbore = initializareArboreDinFisier("studenti.txt");
    afisareArbore(arbore);

    Student s = getStudentByMedie(arbore, 5.59);
    printf("\nstudentul cautat este: ");
    afisareStudent(s);

    printf("\nmedia generala a studentilor este: %.2f\n", calculeazaMedieGenerala(arbore));

    dezalocareArbore(&arbore);
    afisareArbore(arbore);

    return 0;
}