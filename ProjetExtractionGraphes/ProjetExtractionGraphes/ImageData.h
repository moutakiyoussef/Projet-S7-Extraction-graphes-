#pragma once
#ifndef DEF_IMAGEDATA
#define DEF_IMAGEDATA

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>
#include <vector>
#include <float.h>
/* 2d matrices are handled by 2d vectors. */
#define vec2dd vector<vector<double> >
#define vec2di vector<vector<int> >
#define vec2db vector<vector<bool> >
/* The number of iterations run by the clustering algorithm. */
#define NR_ITERATIONS 10

using namespace std;
using namespace cv;
class ImageData
{
public:
	/* The cluster assignments and distance values for each pixel. */
	vec2di clusters;
	vec2dd distances;

	/* The LAB and xy values of the centers. */
	vec2dd centers;
	/* The number of occurences of each center. */
	vector<int> center_counts;

	/* The step size per cluster, and the colour (nc) and distance (ns)
	* parameters. */
	int step, nc, ns;

	ImageData();
	~ImageData();
};

#endif
