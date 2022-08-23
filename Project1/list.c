/*
leap.c

Implementations for leap list construction and manipulation.

Skeleton written by Grady Fitzaptrick for COMP20007 Assignment 1 2022
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "leap.h"
#include "utils.h"

typedef struct node node_t;
struct node {
    int data;
    node_t *next;
    node_t *down;
    node_t *up;
};

typedef struct leapList leapList_t;
struct leapList {
    /* IMPLEMENT: Fill in structure. */
    node_t *head;
    double p;
};

void printList(node_t* head);
void printWholeList(leapList_t *list);

struct leapList *newList(int maxHeight, double p, enum problemPart part){
    /* IMPLEMENT: Set up list */
    int currHeight = 0;
    struct leapList *newList = NULL;
    struct leapList *nextList = NULL;
    node_t *currHead = NULL;
    node_t *nextHead = NULL;
    /* Generate a new list */
    newList = malloc(sizeof(struct leapList));
    assert(newList!=NULL);
    currHeight++;
    newList->head = malloc(sizeof(node_t));
    newList->head->next = NULL;
    newList->p = p;
    /* Generate a series of list and stack them up to maxHeight levels */
    for (; currHeight < maxHeight; currHeight++) {
        /* Generate a new list */
        nextList = malloc(sizeof(struct leapList));
        assert(newList!=NULL);
        nextList->head = malloc(sizeof(node_t));
        assert(nextList!=NULL);
        nextList->head->next = NULL;
        nextList->p = p;
        /* stack lists */
        currHead = newList->head;
        nextHead = nextList->head;
        currHead->up = nextHead;
        nextHead->down = currHead;
        newList->head = nextHead;
    }

    return newList;
}

void printLevel(struct leapList *list, int level){
    if(! list){
        printf("\n");
        return;
    }
    /* IMPLEMENT (Part B): loop over list at given level, printing out each value. */
    /* Note: while additional next elements, print a space after the key. If no additional next elements,
    print a new line and then return. */
    node_t *head = list->head;
    /* Traverse to base level */
    while(head->down != NULL){
        head = head->down;
    }
    /* Traverse up to designated level */
    for (int curr_level = 0; curr_level < level; curr_level++){
        head = head->up;
    }
    /* print level */
    head = head->next;
    if (head == NULL) printf("\n"); // list is empty
    while (head != NULL){
        printf("%d", head->data);
        if (head->next == NULL){  // all elements in list printed
            printf("\n");
            return;
        }
        printf(" ");
        head = head->next;
    }
    return;
}

void insertKey(int key, struct leapList *list){
    /* IMPLEMENT: Insert the key into the given leap list. */
    double p = list->p;
    node_t *head = list->head;
    node_t *baseHead, *next, *newNode, *secondNewNode;
    /* first insert in base list */
    while(head->down != NULL){  // Travel to base list
        head = head->down;
    }
    /* Insert data */
    baseHead = head;
    while(baseHead != NULL) {
        if (baseHead->next == NULL) { // Insert right after head if list is empty
            newNode = malloc(sizeof(node_t));
            newNode->data = key;
            newNode->next = NULL;
            baseHead->next = newNode;
            break;
        }
        if (baseHead->next->data > key){ // Otherwise, insert in order
            newNode = malloc(sizeof(node_t));
            newNode->data = key;
            newNode->next = baseHead->next;
            baseHead->next = newNode;
            break;
        }
        baseHead = baseHead->next;
    }
    /* insert in upper layer(s) */
    while (head != NULL) {
        int threshold = (double) rand() / RAND_MAX < p;
        if (threshold) { // Node should be inserted on an upper level
            head = head->up;
            baseHead = head;
            /* make new node */
            secondNewNode = malloc(sizeof(node_t));
            assert(secondNewNode != NULL);
            secondNewNode->data = key;
            secondNewNode->down = newNode;
            /* insert in a certain layer */
            while (baseHead != NULL) {
                if (baseHead->next == NULL) { // Insert right after head if list is empty
                    baseHead->next = secondNewNode;
                    secondNewNode->next = NULL;
                    break;
                }
                if (baseHead->next->data > key) { // Otherwise, insert in order
                    secondNewNode->next = baseHead->next;
                    baseHead->next = secondNewNode;
                    break;
                }
                baseHead = baseHead->next;
            }
            newNode = secondNewNode;
            if (head->up == NULL) break;
        }
        else { // Key should not be inserted in upper layer
            break;
        }
    }
    //printWholeList(list);
}

void printList(node_t* head){
    /* This function prints out a linked list */
    while(head != NULL){
        printf("%d ", head->data);
        head = head->next;
    }
}

void printWholeList(leapList_t *list){
    /* This function prints out the whole leap list */
    node_t *head = list->head;
    while(head != NULL){
        printList(head);
        printf("\n");
        head = head->down;
    }
    printf("----------------\n");
}

/* Queries the leap list for the given key and places the result in the solution structure. */
int findKey(int key, struct leapList *list, enum problemPart part, struct solution *solution){
    int found = NOTFOUND;
    int element = key;
    int baseAccesses = 0;
    int requiredAccesses = 0;
    int prevAccess = -1;
    assert(solution);
    /* IMPLEMENT: Find the given key in the leap list. */
    node_t* head;
    head = list -> head;
    while(head != NULL){
        if (head->data == key){ // Key found
            found = 1;
            break;
        }
        if (head->next == NULL){ // Reached the end of a level
            head = head->down;

        }
        else{
            if (head->next->data <= key){ // Move to the next element
                head = head->next;
                requiredAccesses++;
                prevAccess = -1;
            }
            else if (head->data == key){ // Found key
                found = 1;
                break;
            }
            else if (head->next->data > key){ // Move down one level since key is impossible in this level
                if (head->next->data == prevAccess){
                    /* Don't need to access since already accessed on the previous level. */
                    requiredAccesses--;
                }
                prevAccess = head->next->data;
                head = head->down;
                requiredAccesses++;
            }
        }
    }
    /* Find base accesses */
    head = list->head;
    /* Travel to the base list */
    while (head->down != NULL){
        head = head->down;
    }
    /* find element in base list */
    while (head != NULL){
        head = head->next;
        if (head == NULL) break; // reached the end of the list
        baseAccesses++;
        if (head->data >= key){
            break;
        }
    }


    /* Insert result into solution. */
    (solution->queries)++;
    solution->queryResults = (int *) realloc(solution->queryResults, sizeof(int) * solution->queries);
    assert(solution->queryResults);
    (solution->queryResults)[solution->queries - 1] = found;
    solution->queryElements = (int *) realloc(solution->queryElements, sizeof(int) * solution->queries);
    assert(solution->queryElements);
    solution->queryElements[solution->queries - 1] = element;
    solution->baseAccesses = (int *) realloc(solution->baseAccesses, sizeof(int) * solution->queries);
    assert(solution->baseAccesses);
    solution->baseAccesses[solution->queries - 1] = baseAccesses;
    solution->requiredAccesses = (int *) realloc(solution->requiredAccesses, sizeof(int) * solution->queries);
    assert(solution->requiredAccesses);
    solution->requiredAccesses[solution->queries - 1] = requiredAccesses;
    return found;
}

void deleteKey(int key, struct leapList *list, enum problemPart part){
    /* IMPLEMENT: Remove the given key from the leap list. */
    node_t *head = list->head;
    node_t *baseHead, *prev;
    /* search key on every level */
    while(head != NULL){
        if (head == NULL) break; // list is empty
        baseHead = head;
        while (baseHead != NULL){
            prev = baseHead;
            baseHead = baseHead->next;
            if (baseHead == NULL) break; // list is empty
            if (baseHead->data == key){
                prev->next = baseHead->next;
                free(baseHead);
                break;
            }
        }
        head = head->down;
    }
}


void freeList(struct leapList *list){
    /* IMPLEMENT: Free all memory used by the list. */
    node_t *head = list->head;
    node_t *baseHead, *next, *prevHead;
    /* Travel through every level and free all levels */
    while(head != NULL){
        baseHead = head;
        /* free level */
        while (baseHead != NULL){
            next = baseHead->next;
            free(baseHead);
            baseHead = next;
        }
        head = head->down;
    }
}


void freeSolution(struct solution *solution){
    if(! solution){
        return;
    }
    freeList(solution->list);
    if(solution->queries > 0){
        free(solution->queryResults);
        free(solution->queryElements);
        free(solution->baseAccesses);
        free(solution->requiredAccesses);
    }
    free(solution);
}
