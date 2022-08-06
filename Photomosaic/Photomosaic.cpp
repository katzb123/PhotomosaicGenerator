#include "Photomosaic.h"

Photomosaic::Photomosaic(int numSegs)
{
	this->numSegs = numSegs;
}
Photomosaic::Photomosaic(int numSegs, int stdThresh, int maxRec)
{
	this->numSegs = numSegs;
	this->stdThresh = stdThresh;
	this->maxRec = maxRec;
}
Photomosaic::~Photomosaic() {}

void Photomosaic::getImgStats(vector<array<MyPoint, 2>> *imgStats, Mat img)//change to pass by pointer l8er!!!!!!!!!!!!!!!!!!!!!!!!
{
	Scalar avg;
	Scalar std;
	int rowInc = img.rows / this->numSegs;
	int colInc = img.cols / this->numSegs;
	int idx = 0;
	for (int i = 0; i < img.rows; i += rowInc) {
		for (int j = 0; j < img.cols; j += colInc) {
			Mat subSec = img(Range(i, i + rowInc - 1), Range(j, j + colInc - 1));
			meanStdDev(subSec, avg, std);
			(*imgStats)[idx][0] = MyPoint(avg.val[0], avg.val[1], avg.val[2]);		//put averages in column 0
			(*imgStats)[idx][1] = MyPoint(std.val[0], std.val[1], std.val[2]);		//put stdev in column 1
			idx++;
		}
	}

}

void Photomosaic::findBestIndexes(kdt::KDTree<MyPoint> *kdtree, vector<int> *bestImgIndexes)
{
	int x = 0;
}

void Photomosaic::buildFixedSizeMosaic(ImageDatastore *imds, vector<int> *bestImgIndexes, Mat photoMosaic)
{
	int rowInc = photoMosaic.rows / this->numSegs;
	int colInc = photoMosaic.cols / this->numSegs;
	int idx = 0;
	for (int i = 0; i < photoMosaic.rows; i += rowInc) {
		for (int j = 0; j < photoMosaic.cols; j += colInc) {
			Counter::printCount();
			Mat bestImg = imds->readImage((*bestImgIndexes)[idx], Size(rowInc, colInc));
			bestImg.copyTo(photoMosaic(Rect(j, i, colInc, rowInc)));
			idx++;
		}
	}
}

Mat Photomosaic::buildStdevMosaic(ImageDatastore *imds, Mat img, kdt::KDTree<MyPoint> *kdtree, int numRec)//change to pass by pointer l8er!!!!!!!!!!!!!!!!!!!!!!!!
{
	Mat toReturn = img;

	vector<array<MyPoint, 2>> imgStats(this->numSegs*this->numSegs);
	getImgStats(&imgStats, img);

	int r = img.rows;
	int c = img.cols;
	int currRow = 0;
	int currCol = 0;

	//setup variables for later
	int rStart = 0;
	int rStop = 0;
	int cStart = 0;
	int cStop = 0;
	int index = 0;
	for (int i = 0; i < this->numSegs*this->numSegs; i++) {
		Counter::printCount();
		if (currCol == this->numSegs) {
			currRow++;
			currCol = 0;
		}
		rStart = currRow*r / this->numSegs;
		rStop = (currRow + 1)*r / this->numSegs;
		cStart = currCol*c / this->numSegs;
		cStop = (currCol + 1)*c / this->numSegs;
		if (numRec < this->maxRec && imgStats[i][1].getMean() > this->stdThresh) {	//recurse if the average of stdev of image is > stdevThresh
			Mat subSec = img(Range(rStart, rStop), Range(cStart, cStop));
			Mat dummy = buildStdevMosaic(imds, subSec, kdtree, numRec + 1);
			dummy.copyTo(toReturn(Rect(Point(cStart, rStart), Point(cStop, rStop))));
		}
		else {
			index = kdtree->nnSearch(imgStats[i][0]);
			Mat dummy = imds->readImage(index, Size(cStop - cStart, rStop - rStart));
			dummy.copyTo(toReturn(Rect(Point(cStart, rStart), Point(cStop, rStop))));
		}
		currCol++;
	}
	return toReturn;
}