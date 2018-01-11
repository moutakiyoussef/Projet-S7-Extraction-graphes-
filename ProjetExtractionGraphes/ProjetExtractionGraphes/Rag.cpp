#include "stdafx.h"
#include "Rag.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>





/*
* Destructor. Clear any present data.
*/
Rag::~Rag()
{
}

// Add a node from vertex i to j. 
// first check if i and j exists. If not first add the vertex
// and then add entry of j into adjacency list of i
void Rag::addEdge(int i , int j)
{
	VPTR v_i = existVertex(i);
	VPTR v_j = existVertex(j);
	if (v_i == NULL) {
		v_i = addVertex(i);
	}
	if (v_j == NULL) {
		v_j = addVertex(j);
	}

	NODEPTR *temp = &(v_i->list);
	while (*temp) {
		temp = &(*temp)->next;
	}
	*temp = new(struct node);
	(*temp)->n = v_j;
	(*temp)->next = NULL;
}

// Add a new vertex to the end of the vertex list
VPTR Rag::addVertex(int i)
{
	VPTR temp = new(struct vertex);
	temp->list = NULL;
	temp->i = i;
	temp->next = NULL;

	VPTR *curr = &V;
	while (*curr) {
		curr = &(*curr)->next;
	}
	*curr = temp;
	return temp;
}

// If vertex exis
VPTR Rag::existVertex(int i)
{
	VPTR temp = V;
	while (temp != NULL) {
		if (temp->i == i) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

// list of vertex
void Rag::listVertex()
{
	VPTR temp = V;
	while (temp) {
		cout << temp->i << " ";
		temp = temp->next;
	}
	cout << "\n";
}

// calculate RGB mean value 
void Rag::find_mean_rgb(cv::Mat imag)
{
	
	int cols = imag.cols;
	int rows = imag.rows;
	double meanR, meanG, meanB;
	int i, j, m, l;
	int part = 8;
	int p = 0;
	for (j = 0; j < cols; j = j + cols / part)
	{
		//for (i = 0; i < rows; i++)
		{
			meanR = 0;
			meanG = 0;
			meanB = 0;
			for (m = 0; m < cols / part; m++)
			{
				for (l = 0; l < rows; l++)
				{
					if ((j + m) < cols)
					{	
					
						meanR += (double)imag.at<Vec3b>(l, j + m)[0] / (cols / part) / rows;
						meanG += (double)imag.at<Vec3b>(l, j + m)[1] / (cols / part) / rows;
						meanB += (double)imag.at<Vec3b>(l, j + m)[2] / (cols / part) / rows;
					}
				}
			}
		}
	}


	/*{
		for (i = 0; i < rows; i = i + rows / part)
		{
			meanR = 0;
			meanG = 0;
			meanB = 0;
			for (m = 0; m < cols; m++)
			{
				for (l = 0; l < rows / part; l++)
				{
					if ((i + l) < rows)
					{
						meanR += (double)imag.at<Vec3b>(l + i, m)[0] / (cols / part) / rows;
						meanG += (double)imag.at<Vec3b>(l, m)[1] / (cols / part) / rows;
						meanB += (double)imag.at<Vec3b>(l, m)[2] / (cols / part) / rows;
					}
				}
			}
			
		}
	}*/

	
//printf("d%", "d%", "d%", meanR, meanG, meanB);
//std::getchar();
	//imshow("Display window", imag);
}


