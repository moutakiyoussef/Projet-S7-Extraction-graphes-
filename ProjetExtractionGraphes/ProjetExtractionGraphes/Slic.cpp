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
	clear_data();
}

void Slic::generate_superpixels(IplImage * image, int step, int nc)
{
}

void Slic::create_connectivity(IplImage * image)
{
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


/*
* Initialize the cluster centers and initial values of the pixel-wise cluster
* assignment and distance values
*
* Input : The image(IplImage)
*Output : -
*/
void Slic::init_data(IplImage *image) {
	/* Initialize the cluster and distance matrices */
	for (int i = 0; i < image->width; i++)
	{
		vector<int> cr;
		vector<double> dr;
		for (int j = 0; j < image->height; j++)
		{
			cr.push_back(-1);
			dr.push_back(FLT_MAX);
		}
		clusters.push_back(cr);
		distances.push_back(dr);
	}

	/* Initialize the centers and counters */
	for (int i = step; i < image->width - step / 2; i += step)
	{
		for (int j = step; j < image->height - step / 2; j += step)
		{
			vector<double> center;
			/* Find the local minimum(gradient-wise) */
			CvPoint nc = find_local_minimum(image, cvPoint(i, j));
			CvScalar colour = cvGet2D(image, nc.x, nc.y);

			/* Generate the center vector */
			center.push_back(colour.val[0]);
			center.push_back(colour.val[1]);
			center.push_back(colour.val[2]);
			center.push_back(nc.x);
			center.push_back(nc.y);

			/* Append to vector of centers */
			centers.push_back(center);
			center_counts.push_back(0);
		}
	}
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

/*
* Display the cluster centers
*
* Input : The image to display upon (IplImage*) and the colour (CvScalar)
* Output :
*/
void Slic::display_center_grid(IplImage *image, CvScalar colour) {
	for (int i = 0; i < (int)centers.size(); i++) {
		cvCircle(image, cvPoint(centers[i][3], centers[i][4]), 2, colour, 2);
	}
}


/*
* Display a single pixel wide contour around the clusters.
*
* Input : The target image (IplImage*) and contour colour (CvScalar).
* Output: -
*/
void Slic::display_contours(IplImage *image, CvScalar colour) {
	const int dx8[8] = { -1, -1,  0,  1, 1, 1, 0, -1 };
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1 };

	/* Initialize the contour vector and the matrix detailing whether a pixel
	* is already taken to be a contour. */
	vector<CvPoint> contours;
	vec2db istaken;
	for (int i = 0; i < image->width; i++) {
		vector<bool> nb;
		for (int j = 0; j < image->height; j++) {
			nb.push_back(false);
		}
		istaken.push_back(nb);
	}

	/* Go through all the pixels. */
	for (int i = 0; i < image->width; i++) {
		for (int j = 0; j < image->height; j++) {
			int nr_p = 0;

			/* Compare the pixel to its 8 neighbours. */
			for (int k = 0; k < 8; k++) {
				int x = i + dx8[k], y = j + dy8[k];

				if (x >= 0 && x < image->width && y >= 0 && y < image->height) {
					if (istaken[x][y] == false && clusters[i][j] != clusters[x][y]) {
						nr_p += 1;
					}
				}
			}

			/* Add the pixel to the contour list if desired. */
			if (nr_p >= 2) {
				contours.push_back(cvPoint(i, j));
				istaken[i][j] = true;
			}
		}
	}

	/* Draw the contour pixels. */
	for (int i = 0; i < (int)contours.size(); i++) {
		cvSet2D(image, contours[i].y, contours[i].x, colour);
	}
}

/*
* Give the pixels of each cluster the same colour values. The specified colour
* is the mean RGB colour per cluster.
*
* Input : The target image (IplImage*).
* Output: -
*/
void Slic::colour_with_cluster_means(IplImage *image) {
	vector<CvScalar> colours(centers.size());

	/* Gather the colour values per cluster. */
	for (int i = 0; i < image->width; i++) {
		for (int j = 0; j < image->height; j++) {
			int index = clusters[i][j];
			CvScalar colour = cvGet2D(image, j, i);

			colours[index].val[0] += colour.val[0];
			colours[index].val[1] += colour.val[1];
			colours[index].val[2] += colour.val[2];
		}
	}

	/* Divide by the number of pixels per cluster to get the mean colour. */
	for (int i = 0; i < (int)colours.size(); i++) {
		colours[i].val[0] /= center_counts[i];
		colours[i].val[1] /= center_counts[i];
		colours[i].val[2] /= center_counts[i];
	}

	/* Fill in. */
	for (int i = 0; i < image->width; i++) {
		for (int j = 0; j < image->height; j++) {
			CvScalar ncolour = colours[clusters[i][j]];
			cvSet2D(image, j, i, ncolour);
		}
	}
}
