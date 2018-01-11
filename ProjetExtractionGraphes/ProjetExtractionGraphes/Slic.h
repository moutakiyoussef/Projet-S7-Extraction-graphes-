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


#include <stdio.h>
#include "ImageData.h"
using namespace std;



/*
* class Slic.
*
* In this class, an over-segmentation is created of an image, provided by the
* step-size (distance between initial cluster locations) and the colour
* distance parameter.
*/
class Slic : public ImageData {
private:

	/* Compute the distance between a center and an individual pixel. */
	double compute_distance(int ci, CvPoint pixel, CvScalar colour);

	/* Find the pixel with the lowest gradient in a 3x3 surrounding. */
	CvPoint find_local_minimum(IplImage *image, CvPoint center);

	/* Remove and initialize the 2d vectors. */
	void clear_data();
	void initialize_data(IplImage *image);

public:

	/* Class constructors and deconstructors. */
	Slic();
	~Slic();

	/* Generate an over-segmentation for an image. */
	void generate_superpixels(IplImage *image, int step, int nc);
	/* Enforce connectivity for an image. */
	void create_connectivity(IplImage *image);

	/* Draw functions. Resp. displayal of the contours. */
	void display_contours(IplImage *image, CvScalar colour);
	
	/* Draw functions. Resp. displayal of the centers. */
	void display_center_grid(IplImage *image, CvScalar colour);
	
	/*Storage Graph*/
	void stockageGraph();

	/* Draw functions. Resp. displayal of the graph. */
	void display_graph(IplImage *image, CvScalar colour);
};

#endif