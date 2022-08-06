#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <array>
class MyPoint : public std::array<double, 3>
{
public:
	static const int DIM = 3;
	// the constructors
	MyPoint() {}
	MyPoint(double R, double G, double B)
	{
		(*this)[0] = R;
		(*this)[1] = G;
		(*this)[2] = B;
	}

	double getMean()
	{
		return (((*this)[0] + (*this)[1] + (*this)[2]) / 3.0);
	}

	// conversion to OpenCV Point2d
	operator Point3d() const { return cv::Point3d((*this)[0], (*this)[1], (*this)[2]); }

};