#include "utils.h"


// Convolve an image with a separable convolution kernel
//
SDoublePlane convolve_separable(const SDoublePlane &input, const SDoublePlane &row_filter, const SDoublePlane &col_filter)
{
	SDoublePlane output(input.rows(), input.cols());
	int num_filter_rows = col_filter.rows();
	int num_filter_cols = row_filter.cols();
	int num_input_cols = input.cols();
	int num_input_rows = input.rows();
	int sum = 0;
	int k=0;

	if ((input.rows() - col_filter.rows()) * (input.cols() - row_filter.cols()) < 0) {
		throw "Mismatched dimensions.";
	}
		
	if (col_filter.rows()%2 == 0 || row_filter.cols()%2 == 0){
		throw "Expected an odd number of rows and columns in the filter";
	}
	/*for(int p=0;p<5;p++)
			for(int q=0;q<5;q++)
				{
					output[p][q] = 0;
					if (p!= 0 && q!= 0 && p!=4 && q!=4)
					{
						input[p][q] = 16;			
					}
					else
						input[p][q] = 0;
				}*/
	
	//convolving first by the column filter	
	for(int i=0; i<= (num_input_rows - num_filter_rows); i++)
	{
		for(int m=0; m < num_input_cols; m++)
		{
			sum = 0;
			k = i;
			for(int j=0; j<num_filter_rows; j++)
			{
				sum += col_filter[j][0] * input[k][m];
				k++;
			}
			output[(k+i)/2][m] = sum;
		}
	}
	
	//convolving by row filter
	for(int i=0; i< num_input_rows;i++)
		{
			for(int m=0; m <= (num_input_cols - num_filter_cols); m++)	
			{	sum = 0;
				k = m;
				for(int j=0;j<num_filter_cols;j++)
				{
					sum += row_filter[0][j] * output[i][k];
					k++;
				}
				output[i][(k+m)/2] = sum;
			}
		}


	return output;
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
