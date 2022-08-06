#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <array>
#include "ImageDatastore.h"
#include "kd-tree-master/kdtree.h"
#include "MyPoint.h"
#include "Helpers.h"

using namespace std;
using namespace cv;

class Photomosaic
{
	int numSegs;
	int stdThresh;
	int maxRec;

public:
	Photomosaic(int numSegs);
	Photomosaic(int numSegs, int stdThresh, int maxRec);
	~Photomosaic();

	void getImgStats(vector<array<MyPoint, 2>> *imgStats, Mat img);
	void findBestIndexes(kdt::KDTree<MyPoint> *kdtree, vector<int> *bestImgIndexes);
	void buildFixedSizeMosaic(ImageDatastore *imds, vector<int> *bestImgIndexes, Mat photoMosaic);
	Mat buildStdevMosaic(ImageDatastore *imds, Mat img, kdt::KDTree<MyPoint> *kdtree, int numRec);


};