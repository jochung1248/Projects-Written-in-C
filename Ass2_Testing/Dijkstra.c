// Dijkstra ADT implementation
// COMP2521 Assignment 2
// z5308483 Joseph Chung

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "PQ.h"

#define INFINITY 1000000

static void replace(int u, int v, PredNode **predList);
static void add(int u, int v, PredNode **predList); 
static void reachable(ShortestPaths sps, Graph g);

ShortestPaths dijkstra(Graph g, Vertex src) {
    // initialise sps
    ShortestPaths sps;
    // malloc distance array of sps
    sps.dist = malloc(sizeof(int) * (GraphNumVertices(g)));
    assert((sps.dist) != NULL);
    // malloc predecessor array of sps
    sps.pred = malloc(sizeof(struct PredNode) * (GraphNumVertices(g)));
    assert((sps.pred) != NULL);

    // set sps source as given source
    sps.src = src;
    // set number of nodes as number of vertices in given graph
    sps.numNodes = GraphNumVertices(g);

    // make a new priority queue
    PQ pq = PQNew();
    
    // iterate through arrays
    int i = 0;
    while (i < sps.numNodes) {
        // set each element in distance array as INFINITY
        sps.dist[i] = INFINITY;
        // set each element in predeccesor array as NULL
        sps.pred[i] = NULL;
        // insert each 'empty' item into priority queue
        PQInsert(pq, i, sps.dist[i]);

        i++;
    }
    // set source distance as 0 and source predecessor as NULL
    sps.dist[src] = 0;
    sps.pred[src] = NULL;

    // insert source item into priority where it will call PQUpdate function
    // and replace original item
    PQInsert(pq, src, sps.dist[src]);

    // iterate through the priority queue
    while (!PQIsEmpty(pq)) {
        // dequeue first item from queue
        int dItem = PQDequeue(pq);
        Vertex u = dItem;

        // set current as the list of the outgoing edges
        AdjList curr = GraphOutIncident(g, u);
        // iterate through list
        while (curr != NULL) {
            // initialise length with the list item's weight
            int w = curr->weight;
            // if the distances are the same, add predecessor
            if ((sps.dist[u] + w) == (sps.dist[curr->v])) {
                add(u, curr->v, sps.pred);
            // if the previous distance is less than current, replace
            } else if ((sps.dist[u] + w) < (sps.dist[curr->v])) {
                sps.dist[curr->v] = (sps.dist[u]) + w;
                // update the queue
                PQInsert(pq, curr->v, sps.dist[curr->v]);
                // replace the predecessor
                replace(u, curr->v, sps.pred);
            }
            curr = curr->next; 
        }
    }
    // set all nodes that aren't reachable to 0
    reachable(sps,g);
    PQFree(pq);
    
	return sps;
}

// function to check whether a vertex is reachable
static void reachable(ShortestPaths sps, Graph g) {
    int j = 0;
    while (j < GraphNumVertices(g)) {
        if (sps.pred[j] == NULL) {
            sps.dist[j] = 0;
        } 
        j++;
    }
}

// function to add predecessor node to the end of the list
static void add(int u, int v, PredNode **predList) {
    // create a new node
    PredNode *new = malloc(sizeof(PredNode));
    assert(new != NULL);
    new->v = u;
    new->next = NULL;
    
    PredNode *curr = predList[v];
    // iterate until the last node
    while (curr->next != NULL) {
        curr = curr->next;
    }
    // connect the last node to the new node
    curr->next = new;
}

// function to replace current predecessor
static void replace(int u, int v, PredNode **predList) {
	// create a new node
	PredNode *new = malloc(sizeof(PredNode));
	assert(new != NULL);
	new->v = u;
	new->next = NULL;
	// replace the current predecessor with new node
	predList[v] = new;
}


void showShortestPaths(ShortestPaths sps) {
 
	printf("Node %d\n",sps.src);

    int i = 0;
	while (i < sps.numNodes) {
		if (i == sps.src) {
	    	printf("%d: x\n",i);
        } else {
			printf("%d: %d\n", i, sps.dist[i]);
        }
        i++;    
	}
}

void freeShortestPaths(ShortestPaths sps) {
    free(sps.dist);
    int i = 0;
    while (i < sps.numNodes) {
        free(sps.pred[i]);
        i = i + 1;
    }
    free(sps.pred);
}

