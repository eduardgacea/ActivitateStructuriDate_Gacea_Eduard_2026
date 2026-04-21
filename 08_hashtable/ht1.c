#include <stdio.h>
#include <stdlib.h>

typedef struct Student Student;
typedef struct Node Node;
typedef struct List List;
typedef struct HashTable HashTable;

struct Student {
    int id;
};

struct Node {
    Student *student;
    Node *next;
};

struct List {
    Node *first;
};

struct HashTable {
    int capacity;
    List **buckets;
};

// allocation
Student* createStudent(int id);
Node* createNode(int id);
List* createList();
HashTable* createHashTable(int capacity);

// api
void listInsert(List *l, int id);
int hashKey(int id, int capacity);
void hashTableInsert(HashTable *ht, int id);

// logging
void printStudent(Student *s);
void printList(List *l);
void printHashTable(HashTable *ht);

// deallocation
void freeStudent(Student **s);
void freeNode(Node **n);
void freeList(List **l);
void freeHashTable(HashTable **ht);

int main() {
    HashTable *ht = createHashTable(4);

    for(int i = 0 ; i < 30 ; i++) {
        hashTableInsert(ht, i);
    }

    printHashTable(ht);

    freeHashTable(&ht);

    return 0;
}

Student* createStudent(int id) {
    Student *s = malloc(sizeof(Student));
    if(s == NULL) return NULL;
    s->id = id;
    return s;
}

Node* createNode(int id) {
    Node *n = malloc(sizeof(Node));
    if(n == NULL) return NULL;
    Student *s = createStudent(id);
    if(s == NULL) {
        free(n);
        n = NULL;
        return NULL;
    };
    n->student = s;
    n->next = NULL;
    return n;
}

List* createList() {
    List *l = malloc(sizeof(List));
    if(l == NULL) return NULL;
    l->first = NULL;
    return l;
}

HashTable* createHashTable(int capacity) {
    HashTable *ht = malloc(sizeof(HashTable));
    if(ht == NULL || capacity <= 0) return NULL;
    ht->capacity = capacity;
    ht->buckets = malloc(sizeof(List*) * capacity);
    if(ht->buckets == NULL) {
        free(ht);
        ht = NULL;
        return NULL;
    };
    for(int i = 0 ; i < capacity ; i++) {
        ht->buckets[i] = createList();
    }
    return ht;
}

void listInsert(List *l, int id) {
    if(l == NULL) return;

    Node *n = createNode(id);
    if(n == NULL) return;

    if(l->first == NULL) {
        l->first = n;
        return;
    }

    n->next = l->first;
    l->first = n;
}

int hashKey(int id, int capacity) {
    return (31 * id) % capacity;
}

void hashTableInsert(HashTable *ht, int id) {
    if(ht == NULL || ht->capacity <= 0) return;
    int hash = hashKey(id, ht->capacity);
    if(ht->buckets[hash] == NULL) return;
    listInsert(ht->buckets[hash], id);
}

void printStudent(Student *s) {
    if(s == NULL) {
        printf("uninitialized student");
        return;
    }

    printf("student has id %d\n", s->id);
}

void printList(List *l) {
    if(l == NULL) {
        printf("uninitialized list\n");
        return;
    }

    if(l->first == NULL) {
        printf("the list is empty\n");
        return;
    }

    Node *n = l->first;
    while(n) {
        printStudent(n->student);
        n = n->next;
    }
}

void printHashTable(HashTable *ht) {
    if(ht == NULL) {
        printf("uninitialized hash table\n");
        return;
    }

    if(ht->capacity <= 0) {
        printf("the hash table is empty\n");
        return;
    }

    for(int i = 0 ; i < ht->capacity ; i++) {
        if(ht->buckets[i] == NULL) {
            printf("uninitialized bucket\n");
            continue;
        }

        if(ht->buckets[i]->first == NULL) {
            printf("the bucket is empty\n");
            continue;
        }

        printf("bucket %d\n", i);
        printList(ht->buckets[i]);
        printf("----------------\n");        
    }
}

void freeStudent(Student** s) {
    if(!s || !*s) return;

    free(*s);
    *s = NULL;
}

void freeNode(Node **n) {
    if(!n || !*n) return;

    freeStudent(&(*n)->student);
    free(*n);
    *n = NULL;
}

void freeList(List **l) {
    if(!l || !*l) return;

    Node *n = (*l)->first;
    while(n) {
        Node *temp = n->next;
        freeNode(&n);
        n = temp;
    }

    free(*l);
    *l = NULL;
}

void freeHashTable(HashTable **ht) {
    if(!ht || !*ht) return;

    for(int i = 0 ; i < (*ht)->capacity ; i++) {
        freeList(&(*ht)->buckets[i]);
    }

    free((*ht)->buckets);
    (*ht)->buckets = NULL;

    free(*ht);
    *ht = NULL;
}