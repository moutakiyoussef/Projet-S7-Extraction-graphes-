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
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#include "slic.h"
#include "rag.h"

int main(int argc, char *argv[]) {

	/* Load the image and convert to Lab colour space. */
	IplImage *image = cvLoadImage("../data/exemple.jpg", 1);

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
	slic.display_center_grid(image, CV_RGB(3, 34, 76));
	/* Display graph*/
	slic.display_graph(image, CV_RGB(0, 0, 0));
	/* Show the image result*/
	cvShowImage("result", image);
	cvWaitKey(0);
	/* Save the image result*/
	cvSaveImage("../data/result.png", image);
	/*
	slic.DisplayID(image);*/
	std::getchar();


	//rag.find_mean_rgb(img);
	//printf("%d, %d", width, h);
	
	
	return 0;
}
