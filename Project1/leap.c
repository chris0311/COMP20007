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

void freeSingleList(node_t *head);
node_t *constructNode(int key);
void printList(node_t* head);
void printWholeList(leapList_t *list);

struct leapList *newList(int maxHeight, double p, enum problemPart part){
    /* IMPLEMENT: Set up list */
    int curr_height = 0;
    struct leapList *newList = NULL, *nextList;
    node_t *curr_head, *next_head;
    newList = malloc(sizeof(struct leapList));
    assert(newList!=NULL);
    curr_height++;
    newList->head = malloc(sizeof(node_t));
    newList->head->next = NULL;
    newList->p = p;
    for (curr_height; curr_height < maxHeight; curr_height++) {
        nextList = malloc(sizeof(struct leapList));
        assert(newList!=NULL);
        nextList->head = malloc(sizeof(node_t));
        nextList->head->next = NULL;
        nextList->p = p;
        curr_head = newList->head;
        next_head = nextList->head;
        curr_head->up = next_head;
        next_head->down = curr_head;
        newList->head = next_head;
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
    for (int curr_level = 1; curr_level < level; curr_level++){
        head = head->up;
    }
    /* print level */
    head = head->next;
    while (head != NULL){
        printf("%d", head->data);
        if (head->next == NULL){
            printf("\n");
            return;
        }
        printf(" ");
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
        if (baseHead->next == NULL) { // Insert right after head
            newNode = malloc(sizeof(node_t));
            newNode->data = key;
            newNode->next = NULL;
            baseHead->next = newNode;
            break;
        }
        if (baseHead->next->data > key){
            newNode = malloc(sizeof(node_t));
            newNode->data = key;
            newNode->next = baseHead->next;
            baseHead->next = newNode;
            break;
        }
        baseHead = baseHead->next;
    }
    /* insert in upper layer */
    while (head != NULL) {
        int threshold = (double) rand() / RAND_MAX < p;
        printf("threshold: %d\n", threshold);
        if (threshold) {
            head = head->up;
            printf("Inserting %d\n", key);
            baseHead = head;
            /* make new node */
            secondNewNode = malloc(sizeof(node_t));
            assert(secondNewNode != NULL);
            secondNewNode->data = key;
            secondNewNode->down = newNode;
            /* insert in a certain layer */
            while (baseHead != NULL) {
                if (baseHead->next == NULL) { // Insert right after head
                    baseHead->next = secondNewNode;
                    secondNewNode->next = NULL;
                    break;
                }
                if (baseHead->next->data > key) {
                    secondNewNode->next = baseHead->next;
                    baseHead->next = secondNewNode;
                    break;
                }
                baseHead = baseHead->next;
            }
            newNode = secondNewNode;
            if (head->up == NULL) break;
        }
        else {
            break;
        }
    }
    printWholeList(list);
}


void printList(node_t* head){
    while(head != NULL){
        printf("%d", head->data);
        head = head->next;
    }
}

void printWholeList(leapList_t *list){
    node_t *head = list->head;
    while(head != NULL){
        printList(head);
        printf("\n");
        head = head->down;
    }
}

node_t *constructNode(int key){
    node_t *newNode;
    newNode = malloc(sizeof(node_t));
    newNode->data = key;
    return newNode;
}

/* Queries the leap list for the given key and places the result in the solution structure. */
int findKey(int key, struct leapList *list, enum problemPart part, struct solution *solution){
    int found = NOTFOUND;
    int element = key;
    int baseAccesses = 0;
    int requiredAccesses = 0;
    int prev_access = -1;
    assert(solution);
    /* IMPLEMENT: Find the given key in the leap list. */
    node_t* head;
    head = list -> head;
    while(head != NULL){
        if (head->data == key){
            found = 1;
            break;
        }
        if (head->next == NULL){
            head = head->down;
        }
        else{
            if (head->next->data <= key){ // Move to the next element
                head = head->next;
                requiredAccesses++;
            }
            else if (head->data == key){ // Found key
                found = 1;
                break;
            }
            else if (head->next->data > key){ // Move down one layer
                head = head->down;
                requiredAccesses++;
                if (head->next->data == prev_access){
                    requiredAccesses--;
                }
                prev_access = head->next->data;
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
    while (head != NULL && head->data != key){
        head = head->next;
        baseAccesses++;
        if (head->data > key){
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
    while(head != NULL){
        if (head == NULL) break;
        baseHead = head;
        while (baseHead != NULL){
            prev = baseHead;
            baseHead = baseHead->next;
            if (baseHead == NULL) break;
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
    while(head != NULL){
        baseHead = head;
        while (baseHead != NULL){
            next = baseHead->next;
            free(baseHead);
            baseHead = next;
        }
        prevHead = head;
        head = head->down;
        free(prevHead);
    }
}

void freeSingleList(node_t *head){
    /* Frees a single linked list. */
    node_t *currNode = head;
    node_t *prevNode;
    while(currNode != NULL){
        prevNode = currNode;
        currNode = currNode->next;
        free(prevNode);
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

