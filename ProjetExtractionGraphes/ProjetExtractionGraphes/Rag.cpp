#include "stdafx.h"
#include "Rag.h"

/*
* Constructor. Nothing is done here.
*/
Rag::Rag()
{
}

/*
* Destructor. Clear any present data.
*/
Rag::~Rag()
{
}

/*
* Display the cluster centers
*
* Input : The image to display upon and the colour
* Output :
*/
void Rag::display_center_grid(IplImage *image, CvScalar colour) {
	for (int i = 0; i < (int)centers.size(); i++) {
		cvCircle(image, cvPoint(centers[i][3], centers[i][4]), 2, colour, 1);
	}
}
