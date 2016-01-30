#include "utils.h"

// Convolve an image with a separable convolution kernel
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
					sum += input[x][y] * filter[p+start_row][q+start_col];
				}
			}
			output[i][j] = sum;
		}
	}
	
	return output;
}


