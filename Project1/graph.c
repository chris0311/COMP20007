/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2022
*/
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "graph.h"
#include "utils.h"
#include "pq.h"

#define INITIALEDGES 32
#define INFINITY 999999

struct edge;

/* Definition of a graph. */
struct graph {
    int numVertices;
    int numEdges;
    int allocedEdges;
    struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
    int start;
    int end;
    int cost;
};

struct graph *newGraph(int numVertices){
    struct graph *g = (struct graph *) malloc(sizeof(struct graph));
    assert(g);
    /* Initialise edges. */
    g->numVertices = numVertices;
    g->numEdges = 0;
    g->allocedEdges = 0;
    g->edgeList = NULL;
    return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost){
    assert(g);
    struct edge *newEdge = NULL;
    /* Check we have enough space for the new edge. */
    if((g->numEdges + 1) > g->allocedEdges){
        if(g->allocedEdges == 0){
            g->allocedEdges = INITIALEDGES;
        } else {
            (g->allocedEdges) *= 2;
        }
        g->edgeList = (struct edge **) realloc(g->edgeList,
                                               sizeof(struct edge *) * g->allocedEdges);
        assert(g->edgeList);
    }

    /* Create the edge */
    newEdge = (struct edge *) malloc(sizeof(struct edge));
    assert(newEdge);
    newEdge->start = start;
    newEdge->end = end;
    newEdge->cost = cost;

    /* Add the edge to the list of edges. */
    g->edgeList[g->numEdges] = newEdge;
    (g->numEdges)++;
}

/* Returns a new graph which is a deep copy of the given graph (which must be
  freed with freeGraph when no longer used). */
struct graph *duplicateGraph(struct graph *g){
    struct graph *copyGraph = (struct graph *) malloc(sizeof(struct graph));
    assert(copyGraph);
    copyGraph->numVertices = g->numVertices;
    copyGraph->numEdges = g->numEdges;
    copyGraph->allocedEdges = g->allocedEdges;
    copyGraph->edgeList = (struct edge **) malloc(sizeof(struct edge *) * g->allocedEdges);
    assert(copyGraph->edgeList || copyGraph->numEdges == 0);
    int i;
    /* Copy edge list. */
    for(i = 0; i < g->numEdges; i++){
        struct edge *newEdge = (struct edge *) malloc(sizeof(struct edge));
        assert(newEdge);
        newEdge->start = (g->edgeList)[i]->start;
        newEdge->end = (g->edgeList)[i]->end;
        newEdge->cost = (g->edgeList)[i]->cost;
        (copyGraph->edgeList)[i] = newEdge;
    }
    return copyGraph;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g){
    int i;
    for(i = 0; i < g->numEdges; i++){
        free((g->edgeList)[i]);
    }
    if(g->edgeList){
        free(g->edgeList);
    }
    free(g);
}

int findMinimum(int A[], int n) {
    /* Returns the minimum value in an array */
    int min = 999999;
    for (int i = 0; i < n; i++) {
        if (A[i] < min) {
            min = A[i];
        }
    }
    return min;
}

struct solution *graphSolve(struct graph *g, enum problemPart part,
                            int numRooms, int startingRoom, int bossRoom, int numShortcuts,
                            int *shortcutStarts, int *shortcutEnds, int numHeartRooms, int *heartRooms) {
    struct solution *solution = (struct solution *)
            malloc(sizeof(struct solution));
    assert(solution);

    int numEdges = g->numEdges;
    struct edge *currEdge;
    int dist[numRooms];
    int visited[numRooms];
    int start = startingRoom;
    int minCostNode, minCost, i, j;

    /* Construct adjacency matrix */
    int adjMatrix[numRooms][numRooms];
    for (i = 0; i < numRooms; i++) {
        for (j = 0; j < numRooms; j++) {
            adjMatrix[i][j] = INFINITY; // Assume all nodes unreachable
        }
    }
    /* assign cost to nodes reachable */
    for (i = 0; i < numEdges; i++) {
        currEdge = g->edgeList[i];
        adjMatrix[currEdge->start][currEdge->end] = currEdge->cost;
        /* assign costs with start and end reversed since the graph is undirected */
        adjMatrix[currEdge->end][currEdge->start] = currEdge->cost;
        adjMatrix[i][i] = 0;  // No cost accessing the node itself
    }

    if (part == PART_A) {
        /* IMPLEMENT 2A SOLUTION HERE */
        /* assign costs of reachable node to the distance array */
        for (i = 0; i < numRooms; i++) {
            dist[i] = adjMatrix[start][i]; // distance array for the start node
            visited[i] = 0; // all nodes not visited
        }

        visited[start] = 1;  // Starting room visited
        for (i = 0; i < numRooms; i++) {
            minCost = INFINITY;

            /* find unvisited node with the lowest cost */
            for (j = 0; j < numRooms; j++) {
                if (!visited[j] && minCost > dist[j]) {
                    minCostNode = j;
                    minCost = dist[j];
                }
            }
            visited[minCostNode] = 1; // Visit the room with the lowest cost

            /* update cost for unvisited and reachable nodes */
            for (j = 0; j < numRooms; j++) {
                /* Only update if the cost has been lowered */
                /* Cost must be less than infinity if a node become reachable */
                if (!visited[j] && dist[j] > (dist[minCostNode] + adjMatrix[minCostNode][j])) {
                    dist[j] = dist[minCostNode] + adjMatrix[minCostNode][j];
                }
            }
        }

        solution->heartsLost = dist[bossRoom];
    } else if (part == PART_B) {
        /* IMPLEMENT 2B SOLUTION HERE */
        int allCosts[numShortcuts];
        /* try each shortcut, and set its cost to 1 */
        for (int sCount = 0; sCount < numShortcuts; sCount++) {
            adjMatrix[shortcutStarts[sCount]][shortcutEnds[sCount]] = 1;
            adjMatrix[shortcutEnds[sCount]][shortcutStarts[sCount]] = 1; // Undirected path

            for (i = 0; i < numRooms; i++) {
                dist[i] = adjMatrix[start][i]; // distance array for the start node
                visited[i] = 0; // all nodes not visited
            }

            visited[start] = 1;  // Starting room visited
            for (i = 0; i < numRooms; i++) {
                minCost = INFINITY;

                /* find node with the lowest cost */
                for (j = 0; j < numRooms; j++) {
                    if (!visited[j] && minCost > dist[j]) {
                        minCostNode = j;
                        minCost = dist[j];
                    }
                }
                visited[minCostNode] = 1; // Visit the room with the lowest cost

                /* update cost for unvisited and reachable nodes */
                for (j = 0; j < numRooms; j++) {
                    if (!visited[j] && dist[j] > (dist[minCostNode] + adjMatrix[minCostNode][j])) {
                        dist[j] = dist[minCostNode] + adjMatrix[minCostNode][j];
                    }
                }
            }
            allCosts[sCount] = dist[bossRoom];
            /* delete shortcut form adjacency matrix */
            adjMatrix[shortcutStarts[sCount]][shortcutEnds[sCount]] = INFINITY;
            adjMatrix[shortcutEnds[sCount]][shortcutStarts[sCount]] = INFINITY;
        }

        solution->heartsLost = findMinimum(allCosts, numShortcuts);
    } else {
        /* IMPLEMENT 2C SOLUTION HERE */
        /* set rooms that has a heart to cost 0 */
        for (int hCount = 0; hCount < numHeartRooms; hCount++) {
            for (int row = 0; row < numRooms; row++) {
                if (adjMatrix[row][heartRooms[hCount]] != INFINITY) { // a reachable node
                    adjMatrix[row][heartRooms[hCount]] = 0;
                }
            }
        }
        /* assign costs of reachable node to the distance array */
        for (i = 0; i < numRooms; i++) {
            dist[i] = adjMatrix[start][i]; // distance array for the start node
            visited[i] = 0; // all nodes not visited
        }

        /* Find cost of all other nodes from the starting node */
        visited[start] = 1;  // Starting room visited
        for (i = 0; i < numRooms; i++) {
            minCost = INFINITY;

            /* find node with the lowest cost */
            for (j = 0; j < numRooms; j++) {
                if (!visited[j] && minCost > dist[j]) {
                    minCostNode = j;
                    minCost = dist[j];
                }
            }
            visited[minCostNode] = 1; // Visit the room with the lowest cost

            /* update cost for unvisited and reachable nodes */
            for (j = 0; j < numRooms; j++) {
                if (!visited[j] && dist[j] > (dist[minCostNode] + adjMatrix[minCostNode][j])) {
                    dist[j] = dist[minCostNode] + adjMatrix[minCostNode][j];
                }
            }
        }

        solution->heartsLost = dist[bossRoom];
    }
    return solution;
}
