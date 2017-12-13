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


class Rag : public Slic
{
public:
	/* Class constructors and deconstructors. */
	Rag();
	~Rag();
	/* Draw functions. Resp. displayal of the centers. */
	void display_center_grid(IplImage *image, CvScalar colour);

};
#endif
