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

	size_t 	filter_rows_num = filter.rows(),
			filter_cols_num = filter.cols(),
			image_rows_num = input.rows(),
			image_cols_num = input.cols();
	size_t 	start_row = filter.rows()/2,
			start_col = filter.cols()/2,
			end_row = input.rows() - start_row,
			end_col = input.cols() - start_col;
	for (size_t i = start_row; i < end_row; i++) {
		for (size_t j = start_col; j < end_col; j++) {
			int sum = 0;
			for (size_t p = 0; p < filter_rows_num; p++) {
				for (size_t q = 0; q < filter_cols_num; q++) {
					sum += input[i-filter_rows_num/2+p][j-filter_cols_num/2+q] * filter[p][q];
				}
			}
			output[i+filter_rows_num/2][j+filter_cols_num/2] = sum;
		}
	}
	
	return output;
}


