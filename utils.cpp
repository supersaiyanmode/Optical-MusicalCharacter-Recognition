#include <algorithm>
#include "utils.h"

SDoublePlane threshold(const SDoublePlane& input, double val, int low_val, int high_val) {
	SDoublePlane image(input);
	for (int i=0, rows = image.rows(); i< rows; i++) {
		for (int j=0, cols = image.cols(); j < cols; j++) {
			if (image[i][j] < val) {
				if (low_val == THRESH_RETAIN) {
					// unchanged.
				} else if (low_val == THRESH_ZERO) {
					image[i][j] = 0;
				} else if (low_val == THRESH_MAX) {
					image[i][j] = 255;
				} else {
					image[i][j] = low_val;
				}
			} else {
				if (high_val == THRESH_RETAIN) {
					// unchanged.
				} else if (high_val == THRESH_ZERO) {
					image[i][j] = 0;
				} else if (high_val == THRESH_MAX) {
					image[i][j] = 255;
				} else {
					image[i][j] = high_val;
				}
			}
		}
	}
	return image;
}

double image_max(const SDoublePlane& input) {
	double *start = input.data_ptr();
	double *end = start + input.rows()*input.cols();
	double *pos = std::max_element(start, end);
	return (pos == end)? -1.0: *pos;
}

SDoublePlane normalise(SDoublePlane input) {
	return input * (255.0/image_max(input));
}

SDoublePlane flipxy(SDoublePlane input) {
	for (int i=0; i<input.rows()/2; i++) {
		for (int j=0; j<input.cols()/2; j++) {
			//std::cout<<"Flipping: "<<i<<" "<<j<<std::endl;
			double temp = input[input.rows() - i - 1][input.cols() - j - 1];
			input[input.rows() - i - 1][input.cols() - j - 1] = input[i][j];
			input[i][j] = temp;
		}
	}
	return input;
}

SDoublePlane non_maximum_suppression(const SDoublePlane& input) {

}

// Convolve an image with a separable convolution kernel
//
SDoublePlane convolve_separable(const SDoublePlane &input, const SDoublePlane &row_filter, const SDoublePlane &col_filter)
{
	SDoublePlane output(input.rows(), input.cols());

	output = convolve_general(output, row_filter);
	return convolve_general(output, col_filter);
}

// Convolve an image with a general convolution kernel
//
SDoublePlane convolve_general(const SDoublePlane &input, const SDoublePlane &filter)
{
	//Requires the dimension of the filter to be smaller than the input.
	//Switch the parameters otherwise.
	
	if ((input.rows() - filter.rows()) * (input.cols() - filter.cols()) < 0) {
		throw "Mismatched dimensions.";
	}
	if (input.rows() < filter.rows()) {
		return convolve_general(filter, input);
	}
	
	if (filter.rows()%2 == 0 || filter.cols()%2 == 0){
		throw "Expected an odd number of rows and columns in the filter";
	}
	//From here, we have ensured dimensions of input is larger than the filter.
	
	SDoublePlane output(input.rows(), input.cols());

	int 	filter_rows_num = filter.rows(),
			filter_cols_num = filter.cols(),
			image_rows_num = input.rows(),
			image_cols_num = input.cols();
	int 	start_row = filter.rows()/2,
			start_col = filter.cols()/2,
			end_row = input.rows() - start_row,
			end_col = input.cols() - start_col;

	for (int i = 0; i < image_rows_num; i++) {
		for (int j = 0; j < image_cols_num; j++) {
			int sum = 0;
			for (int p = -start_row; p <= start_row; p++) {
				for (int q = -start_col; q <= start_col; q++) {
					int x = i + p, y = j+q;
					if (0 > x || x >= image_rows_num || 
							0 > y || y >= image_cols_num) {
						if (y < 0) {
							y = -y;
						} else if (y >= image_cols_num) {
							y = 2*image_cols_num - y - 1;
						}

						if (x < 0) {
							x = -x;
						} else if (x >= image_rows_num) {
							x = 2*image_rows_num - x - 1;
						}
					}
					sum += input[x][y] * 
						filter[filter_rows_num - p - start_row - 1]
							  [filter_cols_num - q - start_col - 1];
				}
			}
			output[i][j] = sum;
		}
	}
	
	return output;
}



SDoublePlane fourier(const SDoublePlane& input) {

}
