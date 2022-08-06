#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

using namespace std;
using namespace std::experimental::filesystem;
using namespace cv;

class ImageDatastore
{
	string allImgFilePath;
	string bigImgFilePath;
	vector<path> dirs;

public:
	ImageDatastore();
	~ImageDatastore();
	void init();
	string getPath(int index);
	size_t getSize();
	Mat readImage(int index, Size s);
	Mat readImage(int index);
	Mat getBigImg();
	void getBigImgFilePath();
	void getAllImgFilePath();

};