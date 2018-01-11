#ifndef RAG_H
#define RAG_H
#pragma once
/* slic.h.
*
* Written by: Youssef Moutaki, Cedric Cognard
*
* This file contains the class elements of the class Rag. This class is an
* implementation of the Region adjacency graph algorithm.
*
*/

#include <stdio.h>
#include "ImageData.h"
#include "Slic.h"

// Structure of a vertex
struct vertex {
	int i;
	struct node *list;
	struct vertex *next;
};
typedef struct vertex * VPTR;

// Struct of adjacency list
struct node {
	struct vertex * n;
	struct node *next;
};

typedef struct node * NODEPTR;

class Rag : public ImageData
{


public:

	// list of nodes chained together
	VPTR V;
	Rag() {
		V = NULL;
	}
	~Rag();
	void addEdge(int i, int j);
	VPTR  addVertex(int i);
	VPTR existVertex(int i);
	void listVertex();
	void find_mean_rgb(cv::Mat imag);
	

};
#endif
