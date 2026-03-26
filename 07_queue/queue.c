#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book Book;
typedef struct Node Node;
typedef struct Queue Queue;

struct Book {
    int id;
    char *title;
    int categoriesCount;
    char **categories;
    float price;
};

struct Node {
    Book *book;
    Node *prev;
    Node *next;
};

struct Queue {
    Node *first;
    Node *last;
};

// allocators
Book *createBook(int id, char *title, int categoriesCount, char **categories, float price);
Node *createNode(Book *book);
Queue *createQueue();

// queue specific methods
void put(Queue *queue, Book *book); // inserting at the beginning
Book *get(Queue *queue); // retrieving from the end

// deallocators
void freeBook(Book **book);
void freeNode(Node **node);
void freeQueue(Queue **queue);

// logging
void printBook(Book *book);
void printQueue(Queue *queue);

int main() {
    Queue *q = createQueue();

    Book *b1 = createBook(1, "book 1", 0, NULL, 12.49);
    Book *b2 = createBook(2, "book 2", 0, NULL, 20.99);
    Book *b3 = createBook(3, "book 3", 0, NULL, 24.00);
    Book *b4 = createBook(4, "book 4", 0, NULL, 34.00);
    put(q, b1);
    put(q, b2);
    put(q, b3);
    put(q, b4);

    printQueue(q);
    printf("---------------------------\n");

    Book *b = get(q);
    printBook(b);
    printf("---------------------------\n");

    printQueue(q);
    printf("---------------------------\n");

    return 0;
}

Book* createBook(int id, char *title, int categoriesCount, char **categories, float price) {
    Book *book = malloc(sizeof(Book));
    if(!book) return NULL;

    char *srcTitle = title ? title : "unknown";
    book->title = malloc(strlen(srcTitle) + 1);
    if(!book->title) {
        free(book);
        return NULL;
    }
    strcpy(book->title, srcTitle);

    if(categoriesCount > 0) {
        book->categories = malloc(categoriesCount * sizeof(char *));
        if (!book->categories) {
            free(book->title);
            free(book);
            return NULL;
        }

        for(int i = 0 ; i < categoriesCount ; i++) {
        char *srcCategory = (categories && categories[i]) ? categories[i] : "unknown";
        book->categories[i] = malloc(strlen(srcCategory) + 1);
        if(!book->categories[i]) {
            for(int j = 0 ; j < i ; j++) {
                free(book->categories[j]);
            }
            free(book->categories);
            free(book->title);
            free(book);
            return NULL;
        }

        strcpy(book->categories[i], srcCategory);
        }
    } else {
        book->categories = NULL;
    }

    book->id = id;
    book->categoriesCount = categoriesCount;
    book->price = price;

    return book;
}

Node* createNode(Book *book) {
    if(!book) return NULL;
    Node *node = malloc(sizeof(Node));
    if(!node) return NULL;

    node->book = book;
    node->prev = node->next = NULL;
    
    return node;
}

Queue* createQueue() {
    Queue* queue = malloc(sizeof(Queue));
    if(!queue) return NULL;

    queue->first = queue->last = NULL;

    return queue;
}

void put(Queue *queue, Book *book) {
    if(!queue || !book) return;

    Node *node = createNode(book);
    if(!node) return;

    if(queue->first == NULL) {
        queue->first = queue->last = node;
        return;
    }

    node->next = queue->first;
    queue->first->prev = node;
    queue->first = node;
}

Book *get(Queue *queue) {
    if (!queue || !queue->last) return NULL;

    Node *temp = queue->last;
    Book *book = temp->book;

    if (queue->first == queue->last) {
        queue->first = queue->last = NULL;
    } else {
        queue->last = temp->prev;
        queue->last->next = NULL;
    }

    free(temp);
    return book;
}

void printBook(Book *book) {
    if(!book) {
        printf("uninitialized book\n");
        return;
    }

    printf("title: %s\n", book->title);
    printf("price: %.2f\n", book->price);
    if(book->categoriesCount > 0) {
        printf("categories: ");
        for(int i = 0 ; i < book->categoriesCount - 1 ; i++) {
            printf("%s, ", book->categories[i]);
        }
        printf("%s\n", book->categories[book->categoriesCount - 1]);
    }
}

void printQueue(Queue *queue) {
    if(!queue) {
        printf("uninitialized queue\n");
        return;
    }

    if(queue->first == NULL) {
        printf("empty queue\n");
        return;
    }

    Node *n = queue->first;

    while(n) {
        printBook(n->book);
        n = n->next;
    }
}

void freeBook(Book **book) {
    if(!book || !*book) return;

    Book *temp = *book;
    
    if(temp->categoriesCount > 0) {
        for(int i = 0 ; i < temp->categoriesCount ; i++) {
            free(temp->categories[i]);
            temp->categories[i] = NULL;
        }
        free(temp->categories);
        temp->categories = NULL;
    }

    free(temp->title);
    temp->title = NULL;

    free(*book);
    *book = NULL;
}

void freeNode(Node **node) {
    if(!node || !*node) return;

    Node *temp = *node;

    freeBook(&temp->book);
    temp->prev = temp->next = NULL;

    free(*node);
    *node = NULL;
}

void freeQueue(Queue **queue) {
    if (!queue || !*queue) return;

    Node *n = (*queue)->first;
    while (n) {
        Node *next = n->next;
        freeNode(&n);
        n = next;
    }

    free(*queue);
    *queue = NULL;
}