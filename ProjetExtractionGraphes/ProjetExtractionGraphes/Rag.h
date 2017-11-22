#pragma once
/* slic.h.
*
* Written by: Youssef Moutaki, Cedric Cognard
*
* This file contains the class elements of the class Rag. This class is an
* implementation of the Region adjacency graph algorithm.
*
*/

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <float.h>
using namespace std;
/* 2d matrices are handled by 2d vectors. */
#define vec2dd vector<vector<double> >
#define vec2di vector<vector<int> >
#define vec2db vector<vector<bool> >

class Rag
{
private:
	/* The LAB and xy values of the centers. */
	vec2dd centers;
public:
	/* Class constructors and deconstructors. */
	Rag();
	~Rag();
	/* Draw functions. Resp. displayal of the centers. */
	void display_center_grid(IplImage *image, CvScalar colour);
};

