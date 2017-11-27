#ifndef SLIC_H
#define SLIC_H

/* slic.h.
*
* Written by: Youssef Moutaki, Cedric Cognard
*
* This file contains the class elements of the class Slic. This class is an
* implementation of the SLIC Superpixel algorithm.
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
/* The number of iterations run by the clustering algorithm. */
#define NR_ITERATIONS 10

/*
* class Slic.
*
* In this class, an over-segmentation is created of an image, provided by the
* step-size (distance between initial cluster locations) and the colour
* distance parameter.
*/
class Slic {
private:
	/* The cluster assignments and distance values for each pixel. */
	vec2di clusters;
	vec2dd distances;

	/* The LAB and xy values of the centers. */
	vec2dd centers;
	/* The number of occurences of each center. */
	vector<int> center_counts;

	

/*Youssef*/
	/* Compute the distance between a center and an individual pixel. */
	double compute_distance(int ci, CvPoint pixel, CvScalar colour);

	/* Find the pixel with the lowest gradient in a 3x3 surrounding. */
	CvPoint find_local_minimum(IplImage *image, CvPoint center);

/*C�dric*/
	/* Remove and initialize the 2d vectors. */
	void clear_data();
	void initialize_data(IplImage *image);

public:

	/* The step size per cluster, and the colour (nc) and distance (ns)
	* parameters. */
	int step, nc, ns;

/*Youssef*/
	/* Class constructors and deconstructors. */
	Slic();
	~Slic();

	/* Generate an over-segmentation for an image. */
	void generate_superpixels(IplImage *image, int step, int nc);
	/*Display all centers ID*/
	void DisplayID(IplImage * image);
	/* Enforce connectivity for an image. */
	void create_connectivity(IplImage *image);

/*C�dric*/
	/* Draw functions. Resp. displayal of the contours. */
	void display_contours(IplImage *image, CvScalar colour);
	
};

#endif