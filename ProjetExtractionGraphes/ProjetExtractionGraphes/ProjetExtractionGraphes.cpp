/*
* Main.cpp.
*
* This file creates an over-segmentation of an image based on the SLIC
* algorithm, and connect all edges to have a graph as a result.
*/

#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <float.h>
using namespace std;

#include "slic.h"
#include "rag.h"

int main(int argc, char *argv[]) {
	/* Load the image and convert to Lab colour space. */
	IplImage *image = cvLoadImage("../data/exemple.jpg", 1);

	IplImage *lab_image = cvCloneImage(image);
	cvCvtColor(image, lab_image, CV_BGR2Lab);

	/* Yield the number of superpixels and weight-factors from the user. */
	int width = image->width, h = image->height;
	int nr_superpixels = 400;
	int nc = 40;

	double step = sqrt((width * h) / (double)nr_superpixels);

	
	Rag rag;
	Slic slic;
	slic.generate_superpixels(image, step, nc);
	slic.create_connectivity(image);

	/* Display the contours and show the result. */
	slic.display_contours(image, CV_RGB(255, 0, 0));
	/* Display the centers and show the result. */
	rag.display_center_grid(image, CV_RGB(0, 255, 0));
	cvShowImage("result", image);
	cvWaitKey(0);
	cvSaveImage("../data/result.jpg", image);
	return 0;
}
