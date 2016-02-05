#ifndef __HOUGH_H
#define __HOUGH_H

#include <vector>

#include "SImage.h"

class Line {
	double x1, y1, x2, y2;
	double _slope, _slopeI;
	double _intercept;
	bool _vertical;
public:
	Line(double, double, double, double);

	bool vertical();
	double slope();
	double yintercept();
	double xintercept();
};

class HoughLinesDetector {
public:
	std::vector<Line> find(const SDoublePlane& img);
	
};
#endif
