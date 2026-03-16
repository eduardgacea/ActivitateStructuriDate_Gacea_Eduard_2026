#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Store Store;
typedef struct Node Node;
typedef struct Stack Stack;

struct Store
{
    int id;
    char *owner;
    int numberOfProducts;
    float *prices;
};

struct Node
{
    Store *store;
    Node *next;
};

struct Stack
{
    Node *first;
};

// constructors
Store *createStore(int id, const char *owner, int numberOfProducts, const float *prices);
Node *createNode(Store *store);
Stack *createStack();

// stack specific methods
void push(Stack *stack, Store *store);
Node *pop(Stack *stack);

// logging
void logStore(Store *store);
void logNode(Node *node);
void logStack(Stack *stack);

// deallocators
void freeStore(Store **store);
void freeNode(Node **node);
void freeStack(Stack **stack);

int main()
{
    Stack *stack = createStack();

    const float prices1[] = {1.61, 2.71, 3.14};
    Store *store1 = createStore(1, "john doe", 3, prices1);
    const float prices2[] = {6.7, 1.41};
    Store *store2 = createStore(2, "jane doe", 2, prices2);
    const float prices3[] = {3.41, 2.13, 10.1, 2.21};
    Store *store3 = createStore(3, "john smith", 4, prices3);
    const float prices4[] = {2.11, 8.74, 2.55};
    Store *store4 = createStore(4, "robert b", 3, prices4);

    push(stack, store1);
    push(stack, store2);
    push(stack, store3);
    push(stack, store4);
    logStack(stack);

    Node *node = pop(stack);
    logStore(node->store);
    logStack(stack);
    freeNode(&node);
    logNode(node);

    freeStack(&stack);
    logStack(stack);

    return 0;
}

Store *createStore(int id, const char *owner, int numberOfProducts, const float *prices)
{
    Store *store = malloc(sizeof(Store));
    if (!store)
        return NULL;

    store->id = id;
    store->numberOfProducts = numberOfProducts;

    const char *temp = owner ? owner : "unknown";
    store->owner = malloc(strlen(temp) + 1);
    if (!store->owner)
    {
        free(store);
        return NULL;
    }
    strcpy_s(store->owner, strlen(temp) + 1, temp);

    store->prices = numberOfProducts <= 0 ? NULL : malloc(numberOfProducts * sizeof(float));
    if (!store->prices)
    {
        free(store->owner);
        free(store);
        return NULL;
    }
    for (int i = 0; i < numberOfProducts; i++)
    {
        store->prices[i] = prices[i];
    }

    return store;
}

Node *createNode(Store *store)
{
    if (!store)
        return NULL;
    Node *node = malloc(sizeof(Node));
    if (!node)
        return NULL;

    node->store = store;
    node->next = NULL;

    return node;
}

Stack *createStack()
{
    Stack *stack = malloc(sizeof(Stack));
    if (!stack)
        return NULL;

    stack->first = NULL;

    return stack;
}

void push(Stack *stack, Store *store)
{
    if (!stack || !store)
        return;
    Node *node = createNode(store);
    if (!node)
        return;

    if (stack->first == NULL)
    {
        stack->first = node;
        return;
    }

    node->next = stack->first;
    stack->first = node;
}

Node *pop(Stack *stack)
{
    if (!stack || !stack->first)
        return NULL;

    Node *node = stack->first;
    stack->first = node->next;
    node->next = NULL;

    return node;
}

void logStore(Store *store)
{
    if (!store)
    {
        printf("store uninitialized\n");
        return;
    }

    printf("store with id %d, owned by %s", store->id, store->owner);
    if (store->numberOfProducts != 0)
    {
        printf(" has an inventory of %d items priced: {", store->numberOfProducts);
        for (int i = 0; i < store->numberOfProducts - 1; i++)
        {
            printf("%.2f ", store->prices[i]);
        }
        printf("%.2f}\n", store->prices[store->numberOfProducts - 1]);
    }
    else
    {
        printf(" has no inventory yet\n");
    }
}

void logNode(Node *node)
{
    if (!node)
    {
        printf("node uninitialized\n");
        return;
    }

    logStore(node->store);
}

void logStack(Stack *stack)
{
    if (!stack)
    {
        printf("stack uninitialized\n");
        return;
    }

    if (!stack->first)
    {
        printf("stack is empty\n");
        return;
    }

    Node *n = stack->first;
    while (n)
    {
        logStore(n->store);
        n = n->next;
    }
}

void freeStore(Store **store)
{
    if (!store || !*store)
        return;

    free((*store)->owner);
    (*store)->owner = NULL;
    free((*store)->prices);
    (*store)->prices = NULL;

    free(*store);
    *store = NULL;
}

void freeNode(Node **node)
{
    if (!node || !*node)
        return;

    freeStore(&(*node)->store);

    free(*node);
    *node = NULL;
}

void freeStack(Stack **stack)
{
    if (!stack || !*stack)
        return;

    if (!(*stack)->first)
    {
        free(*stack);
        *stack = NULL;
        return;
    }

    Node *n = (*stack)->first;
    while (n)
    {
        Node *temp = n->next;
        freeNode(&n);

        n = temp;
    }

    free(*stack);
    *stack = NULL;
}