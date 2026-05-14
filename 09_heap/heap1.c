#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book Book;
typedef struct Heap Heap;

struct Book {
    char *title;
    float price;
};

struct Heap {
    Book *books;
    int length;
    int capacity;
};

// allocators
Book createBook(const char *title, float price);
Heap createHeap(int capacity);

// api
void siftDown(Heap heap, int index);
void insertBook(Heap *heap, Book book);
Book extractMinBook(Heap *heap);

// logging
void logBook(Book book);
void logHeap(Heap heap);

// deallocators
void freeBook(Book *book);
void freeHeap(Heap *heap);

int main() {
    Heap heap = createHeap(5);
    
    Book b1 = createBook("1984", 14.99);
    Book b2 = createBook("The Hobbit", 20.19);
    Book b3 = createBook("Crime and Punishment", 35.49);
    Book b4 = createBook("The Stranger", 11.25);
    Book b5 = createBook("A Song of Ice and Fire", 39.12);

    insertBook(&heap, b1);
    insertBook(&heap, b2);
    insertBook(&heap, b3);
    insertBook(&heap, b4);
    insertBook(&heap, b5);

    logHeap(heap);

    Book minBook = extractMinBook(&heap);
    printf("MIN BOOK\n");
    logBook(minBook);
    printf("-------------------\n");

    logHeap(heap);

    freeBook(&b1);
    freeBook(&b2);
    freeBook(&b3);
    freeBook(&b4);
    freeBook(&b5);
    freeBook(&minBook);

    freeHeap(&heap);

    return 0;
}

// allocators
Book createBook(const char *title, float price) {
    Book book = {0};
    
    const char *src = title == NULL ? "unknown" : title;
    book.title = malloc(strlen(src) + 1);
    if(book.title == NULL) return book;
    strcpy(book.title, src);

    book.price = price;

    return book;
}

Heap createHeap(int capacity) {
    Heap heap = {0};
    if(capacity <= 0) return heap;

    heap.books = malloc(capacity * sizeof(Book));
    if(heap.books == NULL) return heap;

    heap.capacity = capacity;
    heap.length = 0;

    return heap;
}

// api
void siftDown(Heap heap, int index) {
    if(heap.books == NULL || index < 0 || index >= heap.length) return;

    int leftChildIndex = 2 * index + 1;
    int rightChildIndex = 2 * index + 2;
    int minPriceIndex = index;

    if(leftChildIndex < heap.length && heap.books[leftChildIndex].price < heap.books[minPriceIndex].price) {
        minPriceIndex = leftChildIndex;
    }

    if(rightChildIndex < heap.length && heap.books[rightChildIndex].price < heap.books[minPriceIndex].price) {
        minPriceIndex = rightChildIndex;
    }

    if(minPriceIndex != index) {
        Book aux = heap.books[minPriceIndex];
        heap.books[minPriceIndex] = heap.books[index];
        heap.books[index] = aux;
        if(minPriceIndex < (heap.length - 2) / 2) {
            siftDown(heap, minPriceIndex);
        }
    }
}

void insertBook(Heap *heap, Book book) {
    if(heap == NULL || heap->books == NULL || book.title == NULL || heap->length >= heap->capacity) return;
    
    Book copy = createBook(book.title, book.price);
    if(copy.title == NULL) return;
    
    heap->books[heap->length++] = copy;

    for(int i = (heap->length - 2) / 2; i >= 0; i--) {
        siftDown(*heap, i);
    }
}

Book extractMinBook(Heap *heap) {
    Book book = {0};
    
    if(heap == NULL || heap->books == NULL || heap->length == 0) return book;

    book = heap->books[0];
    heap->books[0] = heap->books[heap->length - 1];
    heap->books[heap->length - 1] = book;
    heap->length--;

    for(int i = (heap->length - 2) / 2 ; i >= 0 ; i--) {
        siftDown(*heap, i);
    }

    return book;
}

// logging
void logBook(Book book) {
    if(book.title == NULL) {
        printf("title: unknown, price: %.2f\n", book.price);
        return;
    }

    printf("title: %s, price: %.2f\n", book.title, book.price);
}

void logHeap(Heap heap) {
    if(heap.books == NULL) {
        printf("uninitialized heap\n");
        return;
    }

    if(heap.length == 0) {
        printf("empty heap\n");
        return;
    }

    for(int i = 0 ; i < heap.length ; i++) {
        logBook(heap.books[i]);
    }
    printf("-------------------\n");
}

// deallocators
void freeBook(Book *book) {
    if(book == NULL ||book->title == NULL) return;

    free(book->title);
    book->title = NULL;

    book->price = 0;
}

void freeHeap(Heap *heap) {
    if(heap == NULL || heap->books == NULL) return;

    for(int i = 0 ; i < heap->length ; i++) {
        freeBook(&heap->books[i]);
    }

    free(heap->books);
    heap->books = NULL;

    heap->length = heap->capacity = 0;
}