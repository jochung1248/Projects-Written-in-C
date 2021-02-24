// Dijkstra ADT implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "PQ.h"

ShortestPaths dijkstra(Graph g, Vertex src) {
	ShortestPaths sps = {0};
	return sps;
}

void showShortestPaths(ShortestPaths sps) {

}

void freeShortestPaths(ShortestPaths sps) {

}

// #define INFINITY 1000000


GraphShow(g);
/*
ShortestPaths dijkstra(Graph g, Vertex src) {

    assert (g != NULL);

    int pred[g->nV][g->nV];
    int dist[g->nV];

    int i = 0;
    while (i < g->nV) {
        dist[i] = INFINITY;
        pred[i] = -1;
        i++;
    }

    dist[src] = 0;



    Queue q = QueueNew();



    

}
*/