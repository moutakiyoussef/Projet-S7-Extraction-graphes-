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

/*
* Compute the over-segmentation
*
* Input : The Lab image (IplImage*), the stepsize (int), and the weight (int).
*/
void Slic::generate_superpixels(IplImage * image, int step, int nc)
{
	this->step = step;
	this->nc = nc;
	this->ns = step;

	/* Clear previous data (if any), and re-initialize it. */
	clear_data();
	initialize_data(image);

	/* Run EM for 10 iterations (as prescribed by the algorithm). */
	for (int i = 0; i < NR_ITERATIONS; i++) {
		/* Reset distance values. */
		for (int j = 0; j < image->width; j++) {
			for (int k = 0;k < image->height; k++) {
				distances[j][k] = FLT_MAX;
			}
		}

		for (int j = 0; j < (int)centers.size(); j++) {
			/* Only compare to pixels in a 2 x step by 2 x step region. */
			for (int k = centers[j][3] - step; k < centers[j][3] + step; k++) {
				for (int l = centers[j][4] - step; l < centers[j][4] + step; l++) {

					if (k >= 0 && k < image->width && l >= 0 && l < image->height) {
						CvScalar colour = cvGet2D(image, l, k);
						double d = compute_distance(j, cvPoint(k, l), colour);

						/* Update cluster allocation if the cluster minimizes the
						distance. */
						if (d < distances[k][l]) {
							distances[k][l] = d;
							clusters[k][l] = j;
						}
					}
				}
			}
		}

		/* Clear the center values. */
		for (int j = 0; j < (int)centers.size(); j++) {
			centers[j][0] = centers[j][1] = centers[j][2] = centers[j][3] = centers[j][4] = 0;
			center_counts[j] = 0;
		}

		/* Compute the new cluster centers. */
		for (int j = 0; j < image->width; j++) {
			for (int k = 0; k < image->height; k++) {
				int c_id = clusters[j][k];

				if (c_id != -1) {
					CvScalar colour = cvGet2D(image, k, j);

					centers[c_id][0] += colour.val[0];
					centers[c_id][1] += colour.val[1];
					centers[c_id][2] += colour.val[2];
					centers[c_id][3] += j;
					centers[c_id][4] += k;

					center_counts[c_id] += 1;
				}
			}
		}

		/* Normalize the clusters. */
		for (int j = 0; j < (int)centers.size(); j++) {
			centers[j][0] /= center_counts[j];
			centers[j][1] /= center_counts[j];
			centers[j][2] /= center_counts[j];
			centers[j][3] /= center_counts[j];
			centers[j][4] /= center_counts[j];
		}
	}
}

/*
* Enforce connectivity of the superpixels. This part is not actively discussed
* in the paper, but forms an active part of the implementation of the authors
* of the paper.
*
* Input : The image (IplImage*).
*/
void Slic::create_connectivity(IplImage * image)
{
	int label = 0, adjlabel = 0;
	const int lims = (image->width * image->height) / ((int)centers.size());

	const int dx4[4] = { -1,  0,  1,  0 };
	const int dy4[4] = { 0, -1,  0,  1 };

	/* Initialize the new cluster matrix. */
	vec2di new_clusters;
	for (int i = 0; i < image->width; i++) {
		vector<int> nc;
		for (int j = 0; j < image->height; j++) {
			nc.push_back(-1);
		}
		new_clusters.push_back(nc);
	}

	for (int i = 0; i < image->width; i++) {
		for (int j = 0; j < image->height; j++) {
			if (new_clusters[i][j] == -1) {
				vector<CvPoint> elements;
				elements.push_back(cvPoint(i, j));

				/* Find an adjacent label, for possible use later. */
				for (int k = 0; k < 4; k++) {
					int x = elements[0].x + dx4[k], y = elements[0].y + dy4[k];

					if (x >= 0 && x < image->width && y >= 0 && y < image->height) {
						if (new_clusters[x][y] >= 0) {
							adjlabel = new_clusters[x][y];
						}
					}
				}

				int count = 1;
				for (int c = 0; c < count; c++) {
					for (int k = 0; k < 4; k++) {
						int x = elements[c].x + dx4[k], y = elements[c].y + dy4[k];

						if (x >= 0 && x < image->width && y >= 0 && y < image->height) {
							if (new_clusters[x][y] == -1 && clusters[i][j] == clusters[x][y]) {
								elements.push_back(cvPoint(x, y));
								new_clusters[x][y] = label;
								count += 1;
							}
						}
					}
				}

				/* Use the earlier found adjacent label if a segment size is
				smaller than a limit. */
				if (count <= lims >> 2) {
					for (int c = 0; c < count; c++) {
						new_clusters[elements[c].x][elements[c].y] = adjlabel;
					}
					label -= 1;
				}
				label += 1;
			}
		}
	}
}


void Slic::stockageGraph()
{
}

/*
* Compute the distance between a cluster center and an individual pixel.
*
* Input : The cluster index (int), the pixel (CvPoint), and the Lab values of
*         the pixel (CvScalar).
* Output: The distance (double).
*/
double Slic::compute_distance(int ci, CvPoint pixel, CvScalar colour) {

	double distanceCluster = sqrt(pow(centers[ci][0] - colour.val[0], 2) + pow(centers[ci][1]
		- colour.val[1], 2) + pow(centers[ci][2] - colour.val[2], 2));

	double distancePixel = sqrt(pow(centers[ci][3] - pixel.x, 2) + pow(centers[ci][4] - pixel.y, 2));

	return sqrt(pow(distanceCluster / nc, 2) + pow(distancePixel / ns, 2));

}


/*
* Find a local gradient minimum of a pixel in a 3x3 neighbourhood.
*
* Input : The image (IplImage*) and the pixel center (CvPoint).
* Output: The local gradient minimum (CvPoint).
*/
CvPoint Slic::find_local_minimum(IplImage * image, CvPoint center)
{
	double minimumGradient = FLT_MAX;
	CvPoint localMinimum = cvPoint(center.x, center.y);

	for (int i = center.x - 1; i < center.x + 2; i++) {
		for (int j = center.y - 1; j < center.y + 2; j++) {
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
* CLear any data exist
*/
void Slic::clear_data()
{
	clusters.clear();
	distances.clear();
	centers.clear();
	center_counts.clear();
}

/*
* Initialize the cluster centers and initial values of the pixel cluster
* assignment and distance values
*
* Input : The image(IplImage)
*Output : -
*/
void Slic::initialize_data(IplImage *image) {
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
			/* Find the local minimum */
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
* Display a single pixel wide contour around the clusters.
*
* Input : The target image (IplImage*) and contour colour (CvScalar).
* Output: -
*/
void Slic::display_contours(IplImage *image, CvScalar colour) {
	const int dx8[8] = { -1, -1,  0,  1, 1, 1, 0, -1 };
	const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1 };

	vector<CvPoint> contours;
	vec2db istaken;
	for (int i = 0; i < image->width; i++) {
		vector<bool> nb;
		for (int j = 0; j < image->height; j++) {
			nb.push_back(false);
		}
		istaken.push_back(nb);
	}


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
* Display the cluster centers
*
* Input : The image to display upon and the colour
* Output :
*/
void Slic::display_center_grid(IplImage *image, CvScalar colour) {

	for (int i = 0; i < (int)centers.size(); i++) {
		cvCircle(image, cvPoint(centers[i][3], centers[i][4]), 2, colour, 1);
	}
}

/*
* Display the graph
*
* Input : The image to display upon and the colour
* Output :
*/
void Slic::display_graph(IplImage * image, CvScalar colour)
{
	for (int i = 0; i < 360; i++) {
		
			cvLine(image, cvPoint(centers[i][3], centers[i][4]), cvPoint(centers[i+1][3], centers[i+1][4]), colour, 1);
			
	}
}
