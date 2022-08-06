#include "ImageDatastore.h"

ImageDatastore::ImageDatastore(){}
ImageDatastore::~ImageDatastore() {}

void ImageDatastore::init() 
{
	//fs::path p(allImgFilePath);
	std::experimental::filesystem::path p(allImgFilePath);
	if (!exists(p) || !is_directory(p)) {
//		cout << p << " is not a path\n";
	}
	recursive_directory_iterator begin(p), end;
	vector<std::experimental::filesystem::path> paths(begin, end);
	
	//cout << "There are " << paths.size() << " files: \n";
	for (path &d : paths) {
		string test = d.extension().string();
		if (is_regular_file(d) && test == ".jpg") {
			dirs.push_back(d);
		}
	}
}

string ImageDatastore::getPath(int index)
{
	return dirs.at(index).string();
}

size_t ImageDatastore::getSize()
{
	return dirs.size();
}

Mat ImageDatastore::readImage(int index, Size s)
{
	Mat img = imread(dirs.at(index).string(), IMREAD_COLOR);
	if (!img.data) {
		cout << "Couldn't read image at: " << dirs.at(index).string() << endl;
		return Mat::ones(s, CV_8UC3);
	}
	resize(img, img, s);
	return img;
}

Mat ImageDatastore::readImage(int index)
{
	return imread(dirs.at(index).string());
}

Mat ImageDatastore::getBigImg()
{
	return imread(bigImgFilePath, IMREAD_COLOR);
}

void ImageDatastore::getBigImgFilePath()
{
	
	path im(bigImgFilePath);
	while (!exists(im) || !is_regular_file(im) || !(im.extension() == ".jpg")) {
		cout << "Enter file path to image you want to convert into a photomosaic: ";
		cin >> bigImgFilePath;
		im = bigImgFilePath;
	}
	int x = 0;
}

void ImageDatastore::getAllImgFilePath()
{
	
	path p(allImgFilePath);
	while (!exists(p) || !is_directory(p)) {
		cout << "Enter file path to folder of images: ";
		cin >> allImgFilePath;
		p = allImgFilePath;
	}
}
