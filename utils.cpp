#include <algorithm>
#include <numeric>
//#include <algorithm>
#include <iostream>
#include <fstream>
#include "utils.h"
#include "Kernel.h"
#include "SImageIO.h" 	
#include "Canny.h"
#include<cmath>

SDoublePlane score_using_edgemaps(const SDoublePlane& input) //question 5
{

	//generating binary edge maps
	SDoublePlane image = convert_to_binary_edgemap(canny(input, 400, 0));
	SDoublePlane temp = SImageIO::read_png_file("template1.png");
	SDoublePlane image_template = convert_to_binary_edgemap(canny(input, 400, 0));

	//calculating the the distance matrix D
	cout<<"starting distance calculator.."<<endl;
	SDoublePlane D_mat = dist(input);
	cout<<"done with distance"<<endl;

	//calculating the scores
	SDoublePlane dist_score(input.rows(),input.cols());
	cout<<"pushing the distance scores.."<<endl;
	double sum = 0;
	for(int i=0; i<input.rows(); i++)
	{
		for(int j=0; j<input.cols(); j++)
		{	
			sum = 0;
			for(int k=0; (i+k)<input.rows(); k++)
			{
				for(int l=0; (j+l)<input.cols(); l++)
				{
					sum += image_template[k][l] * D_mat[i+k][j+l];	
				}
			}
			dist_score[i][j] = sum;
		}	
	}

	cout<<endl<<"printing the values of the score matrix"<<endl;
	for(int i=0; i<input.rows();i++)
	{	for(int j=0; j<input.cols();j++)
			{
				cout<<dist_score[i][j]<<" ";
			}
		cout<<endl;
	}

	
	//SImageIO::write_png_file("BinarytempTrial.png", image, image, image);
	return input;
}

double f_gamma(double val)
{
	if(val != 0)
		return 0;
	return INT_MAX;
}

SDoublePlane dist(SDoublePlane img)
{
	double min_dist = INT_MAX;
	double temp;
	SDoublePlane D(img.rows(), img.cols()); 
	for(int i=0; i<img.rows(); i++)
	{
		for(int j=0; j<img.cols(); j++)
		{	
			min_dist = INT_MAX;
			for(int a=0; a<img.rows(); a++)
			{
				for(int b=0; b<img.cols(); b++)
				{
					temp = f_gamma(img[a][b]) + sqrt( pow((i-a),2) + pow((j-b),2)) ;
					if(temp < min_dist)
						min_dist = temp;		
				}
			}
			D[i][j] = min_dist;
		}	
	}

	
	
	return D;
}

SDoublePlane convert_to_binary_edgemap(SDoublePlane img)
{
	for (int i=0; i<img.rows(); i++)
	{
		for(int j=0; j<img.cols(); j++)
			if(img[i][j]!=0)
				img[i][j]=1;
	}
	return img;

}

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

double image_sum(const SDoublePlane& input) {
	double *start = input.data_ptr();
	double *end = start + input.rows()*input.cols();
	return std::accumulate(start, end, 0);
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

/*SDoublePlane normalise_kernel(SDoublePlane input) {
	for(int k=0; k<input.rows(); k++){
		for(int l=0; l<input.cols(); l++){
			if(input.rows()%2==0){
			std::cout<<input.rows()<<std::endl;		
			input.rows()+1;
			std::cout<<"New count"<<input.rows()<<std::endl;

			}
			if(input.cols()%2==0)
			input.cols()+1;
		}
	}
	return input;
}
*/
SDoublePlane non_maximum_suppression(const SDoublePlane& input) {
	return input;
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
