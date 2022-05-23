#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "234tree.h"

struct tree {
    struct tree *child0;
    struct tree *child1;
    struct tree *child2;
    struct tree *child3;
    int value1;
    int value2;
    int value3;
    int numVals;
    int numDescendants;
};


int main(int argc, char **argv){
    int value;

    struct tree *tree = NULL;

    while(scanf("%d", &value) > 0){
        tree = insertTree(tree, value);
    }

    traverseTree(tree);

    printTree(tree, 0);

    int totalVal = tree->numVals + tree->numDescendants;
    int targetVal = (totalVal + 1) / 2;

    printf("targetVal: %d totalVal: %d\n", targetVal, totalVal);

    findMedian(tree, totalVal, targetVal, 0);


    return 0;
}


