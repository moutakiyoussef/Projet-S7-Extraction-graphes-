#include "stdafx.h"
#include "Slic.h"


/*
* Constructor. Nothing is done here.
*/
Slic::Slic()
{
}


/*
* Destructor. Clear any present data.
*/
Slic::~Slic()
{	
	//clear_data();
}

/*
* Compute the distance between a cluster center and an individual pixel.
*
* Input : The cluster index (int), the pixel (CvPoint), and the Lab values of
*         the pixel (CvScalar).
* Output: The distance (double).
*/
double Slic::compute_dist(int ci, CvPoint pixel, CvScalar colour) {

	double dc = sqrt(pow(centers[ci][0] - colour.val[0], 2) + pow(centers[ci][1]
		- colour.val[1], 2) + pow(centers[ci][2] - colour.val[2], 2));

	double ds = sqrt(pow(centers[ci][3] - pixel.x, 2) + pow(centers[ci][4] - pixel.y, 2));

	return sqrt(pow(dc / nc, 2) + pow(ds / ns, 2));

	//double w = 1.0 / (pow(ns / nc, 2));
	//return sqrt(dc) + sqrt(ds * w);
}
