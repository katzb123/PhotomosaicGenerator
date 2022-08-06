
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <string>

#include "ImageDatastore.h"
#include "kd-tree-master/kdtree.h"
#include "MyPoint.h"
#include "Helpers.h"
#include "Photomosaic.h"

using namespace cv;
using namespace std;

void getBankStats(ImageDatastore *imds, vector<MyPoint> *stats)
{
	Mat img;
	Scalar avg;
	for (int i = 0; i < imds->getSize(); i++) {	//getstats
		Counter::printCount();
		img = imds->readImage(i);	//doesn't resize (might be faster)
		avg = mean(img);
		(*stats)[i] = MyPoint(avg.val[0], avg.val[1], avg.val[2]);
		
		//cout << i << endl;
	}
}

size_t Counter::count = 0;
size_t Counter::total = 1;

int main(int argc, char** argv)
{

	//Paths

	//const char *allImgFilePath = "C:/Dev/Photomosaic/Photomosaic/101_ObjectCategories";
	//const char *bigImgFilePath = "C:/Dev/Photomosaic/Photomosaic/101_ObjectCategories/water_lilly/image_0004.jpg";
	
	//Setup IMDS
	ImageDatastore imds = ImageDatastore();
	imds.getBigImgFilePath();
	imds.getAllImgFilePath();
	imds.init();

	//get stats for all photos
	vector<MyPoint> stats(imds.getSize());	//vector of length numImages, MyPoint is the RGB point
	cout << "Found " << imds.getSize() << " images." << endl;
	cout << "Getting all image averages." << endl;

	Counter::setTotal(imds.getSize());
	Counter::resetCount();

	getBankStats(&imds, &stats);
	cout << "Done getting bank of image stats." << endl;

	//build kd tree
	kdt::KDTree<MyPoint> kdtree(stats);


	//FLAGS
	bool CREATE_FIXED_SIZE = false;
	bool CREATE_STDEV = false;

	cout << "Would you like to create a fixed size photomosaic (y/n): ";
	if (receaveUserInput() == "y") {
		CREATE_FIXED_SIZE = true;
	}
	cout << "Would you like to create a stdev photomosaic (y/n): ";
	if (receaveUserInput() == "y") {
		CREATE_STDEV = true;
	}

	int bigImgSize;

	if (CREATE_FIXED_SIZE) {

		cout << "What do you want the image size to be (4096): ";
		bigImgSize = stoi(receaveUserInput());
		cout << "How many segments do you want the image to have (128): ";
		int numSegsFixed = stoi(receaveUserInput());

		Counter::setTotal(numSegsFixed*numSegsFixed);
		Counter::resetCount();

		//create photomosaic object
		Photomosaic fsMosaic = Photomosaic(numSegsFixed);

		//get big image stats
		Mat bigImg = imds.getBigImg();
		resize(bigImg, bigImg, Size(bigImgSize, bigImgSize));
		vector<array<MyPoint, 2>> bigImgStats(numSegsFixed*numSegsFixed);
		fsMosaic.getImgStats(&bigImgStats, bigImg);

		//get best image indexes
		vector<int> bestImgIndexes(numSegsFixed*numSegsFixed);
		for (int i = 0; i < numSegsFixed*numSegsFixed; i++) {
			bestImgIndexes[i] = kdtree.nnSearch(bigImgStats[i][0]);
		}

		//FIXED SIZE
		Mat mosaic1 = bigImg;
		fsMosaic.buildFixedSizeMosaic(&imds, &bestImgIndexes, mosaic1);

		path full_path(current_path());
		full_path.append("/fixedSizeMosaic.jpg");

		imwrite(full_path.string(), mosaic1);
		cout << "Done with fixed size mosaic" << endl;

	}
	if (CREATE_STDEV) {

		if (!CREATE_FIXED_SIZE) {
			cout << "What do you want the image size to be (4096): ";
			bigImgSize = stoi(receaveUserInput());
		}
		
		//STDEV
		const int numSegs = 2;
		const int stdThresh = 10;
		const int maxRec = 6;

		Counter::setTotal((size_t) pow(2, maxRec*numSegs + 2));
		Counter::resetCount();


		Photomosaic stdevMosaic = Photomosaic(numSegs, stdThresh, maxRec);


		Mat bigImg = imds.getBigImg();
		resize(bigImg, bigImg, Size(bigImgSize, bigImgSize));
		Mat mosaic2 = bigImg;
		mosaic2 = stdevMosaic.buildStdevMosaic(&imds, mosaic2, &kdtree, 0);

		path full_path(current_path());
		full_path.append("/stdevMosaic.jpg");

		imwrite(full_path.string(), mosaic2);
		cout << "Done with stdev mosaic" << endl;

	}

	return 0;
}