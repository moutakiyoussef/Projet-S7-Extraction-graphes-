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

	double distance = sqrt(pow(centers[ci][0] - colour.val[0], 2) + pow(centers[ci][1]
		- colour.val[1], 2) + pow(centers[ci][2] - colour.val[2], 2));

	double distance = sqrt(pow(centers[ci][3] - pixel.x, 2) + pow(centers[ci][4] - pixel.y, 2));

	return sqrt(pow(distance / nc, 2) + pow(distance / ns, 2));

}


/*
* Find a local gradient minimum of a pixel in a 3x3 neighbourhood. This
* method is called upon initialization of the cluster centers.
*
* Input : The image (IplImage*) and the pixel center (CvPoint).
* Output: The local gradient minimum (CvPoint).
*/
CvPoint Slic::find_local_minimum(IplImage * image, CvPoint center)
{
	double minimumGradient = FLT_MAX;
	CvPoint localMinimum = cvPoint(center.x, center.y);

	for (int i = center.x - 1; i < center.x + 2; i++) {
		for (int j = center.y - 1; i < center.y + 2; j++) {
			CvScalar cs1 = cvGet2D(image, j + 1, i);
			CvScalar cs2 = cvGet2D(image, j, i + 1);
			CvScalar cs3 = cvGet2D(image, j, i);
			//Convert colour values to grayscale values
			double v1 = cs1.val[0];
			double v2 = cs2.val[0];
			double v3 = cs3.val[0];

			/*Compute horizontal and vertical gradient and keep a track of the minimum*/
			if (sqrt(pow(v1 - v3, 2)) + sqrt(pow(v2 - v3, 2)) < minimumGradient) {
				minimumGradient = fabs(v1 - v3) + fabs(v2 - v3);
				localMinimum.x = i;
				localMinimum.y = j;
			}
		}
	}
	return localMinimum;
}
