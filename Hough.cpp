#include <cmath>
#include <cstring>
#include <iostream>

#include "Hough.h"
#include "SImageIO.h"
#include "A1Debug.h"

Line::Line(double _x1, double _y1, double _x2, double _y2): 
						x1(_x1), y1(_y1), x2(_x2), y2(_y2) {
	double deltaX = x2 - x1, deltaY = y2 -  y1;
	_vertical = std::abs(deltaX) < std::abs(deltaY);
	_slopeI = _slope = 0.0;
	if (_vertical) {
		_slopeI = deltaY / deltaX;
	} else {
		_slope = deltaX / deltaY;
	}
	_intercept = _vertical? x1 - y1 * _slopeI : y1 - x1 * _slope;
}

double Line::slope() {
	return _vertical? _slopeI: _slope;
}

double Line::yintercept() {
	return _intercept;
}

double Line::xintercept() {
	return 0;
}

bool Line::vertical() {
	return _vertical;
}

HoughLinesDetector::HoughLinesDetector(double thresh): _threshold(thresh) {
	
}

std::vector<int> HoughLinesDetector::find(const SDoublePlane& input) {
	debug_png("hough_input.png", input);
	int rows = input.rows(), cols = input.cols();

	int angle_bins = 512;
	double max_angle = M_PI;
	int r_bins = std::min(input.rows(), 512);
	int max_r = 2*(int)(std::sqrt(rows*rows + cols*cols)+1) + 2;

	SDoublePlane hough(r_bins, angle_bins);

	//memset zero on a double array: Thug life! :P
	std::memset(hough.data_ptr(), 0, hough.rows() * hough.cols() * sizeof(double));

	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			if (input[i][j] > 254) {
				for (int angle_step = 0; angle_step < angle_bins; angle_step++) {
					double angle = max_angle * angle_step / (float)angle_bins;
					double r = i * std::cos(angle) + j * std::sin(angle);

					int row = (int)((r / (max_r/2.0) + 1.0)*r_bins/2.0);
					int col = angle_step;
					hough[row][col] += 1;
				}
			}
		}
	}
	hough = normalise(hough);
	debug_png("hough_output.png", hough);

	hough = threshold(hough, _threshold, THRESH_ZERO, THRESH_RETAIN);
	//hough = non_maximum_suppression(hough);
	
	double max_line_length = std::sqrt(rows*rows + cols*cols);
	std::vector<int> lines;
	for (int i=0; i<hough.rows(); i++) {
		for (int j=0; j<hough.cols(); j++) {
			if (!hough[i][j])
				continue;
			double r = (2.0 * i / r_bins - 1.0) * max_r / 2.0;
			double theta = max_angle * j / angle_bins;
			std::cout<<"Found a line with parameter: r="<<r<<", angle="<<theta<<std::endl;

			if (std::abs(theta) < 0.01)
				lines.push_back(r);
		}
	}

	return lines;
}

