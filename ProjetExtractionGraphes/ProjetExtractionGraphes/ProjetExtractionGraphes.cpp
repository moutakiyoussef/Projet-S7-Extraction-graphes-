/*
* Main.cpp.
*
* This file creates an over-segmentation of a provided image based on the SLIC
* superpixel algorithm, as implemented in slic.h and slic.cpp.
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

int main(int argc, char *argv[]) {
	/* Load the image and convert to Lab colour space. */
	IplImage *image = cvLoadImage("../data/exemple1.jpg", 1);

	IplImage *lab_image = cvCloneImage(image);
	cvCvtColor(image, lab_image, CV_BGR2Lab);

	/* Yield the number of superpixels and weight-factors from the user. */
	int w = image->width, h = image->height;
	int nr_superpixels = 400;
	int nc = 40;

	double step = sqrt((w * h) / (double)nr_superpixels);

	/* Perform the SLIC superpixel algorithm. */
	Slic slic;
	slic.generate_superpixels(lab_image, step, nc);
	slic.create_connectivity(lab_image);

	/* Display the contours and show the result. */
	slic.display_contours(image, CV_RGB(255, 0, 0));
	cvShowImage("result", image);
	cvWaitKey(0);
	cvSaveImage("../data/result1.jpg", image);
	return 0;
}
