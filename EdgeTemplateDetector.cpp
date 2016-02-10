#include <cmath>
#include <iostream>
#include <climits>

#include "EdgeTemplateDetector.h"
#include "Kernel.h"
#include "utils.h"
#include "SImageIO.h"
#include "A1Debug.h"

namespace {
	SDoublePlane convert_to_binary_edgemap(SDoublePlane img) {
		for (int i=0; i<img.rows(); i++) {   
			for(int j=0; j<img.cols(); j++) {
				if(img[i][j]!=0)
					img[i][j]=1;
			}
		}
		return img;
	}

	SDoublePlane dist(SDoublePlane img) {
		double min_dist = INT_MAX;
		double temp;
		double gamma_val;
		SDoublePlane D(img.rows(), img.cols());
		for(int i=0; i<img.rows(); i++) {
			for(int j=0; j<img.cols(); j++) {
				std::cout<<"Processing Dist Map: "<<i<<" "<<j<<std::endl;
				min_dist = INT_MAX;
				if(img[i][j]>0) {
					D[i][j] = 0;
					continue;
				}

				for(int a=0; a<img.rows(); a++) {
					for(int b=0; b<img.cols(); b++)
					{
						if( i==a && j==b)
							continue;
						if(img[a][b] != 0)
							gamma_val = 0;
						else
							gamma_val = INT_MAX;
						temp = gamma_val + std::sqrt( ((i-a)*(i-a)) + ((j-b)*(j-b))) ;
						if(temp < min_dist)
							min_dist = temp;    
					}
				}
				D[i][j] = min_dist;
			}
		}   

		return D;
	}
	double f_gamma(double val) {
		if(val != 0)
			return 0;
		return INT_MAX;
	}

	SDoublePlane get_sobel(SDoublePlane input) {
		SDoublePlane sobelx_kernel = load_kernel("kernels/sobel3x");
		SDoublePlane sobely_kernel = load_kernel("kernels/sobel3y");

		SDoublePlane sobelx = convolve_general(input, sobelx_kernel);
		SDoublePlane sobely = convolve_general(input, sobely_kernel);

		SDoublePlane ret = sobelx + sobely;
		debug_png("Sobel_temp.png", ret);
		return ret;
	}

	std::vector<DetectedSymbol> score_using_edgemaps(const SDoublePlane& input) {

		//generating binary edge maps using canny
		//SDoublePlane image_binary_edgemap = convert_to_binary_edgemap(canny(input, 400, 0));
		SDoublePlane temp = SImageIO::read_png_file("template2.png");
		//SDoublePlane template_binary_edgemap = convert_to_binary_edgemap(canny(temp, 400, 0));
		SDoublePlane template_binary_edgemap = convert_to_binary_edgemap(get_sobel(temp));

		//edgemaps using soble
		SDoublePlane image_binary_edgemap = convert_to_binary_edgemap(get_sobel(input));
		(void)get_sobel(temp);
		SDoublePlane temp_image_binary_edgemap = threshold(image_binary_edgemap, 0.5, THRESH_ZERO, THRESH_MAX);
		SDoublePlane temp_template_binary_edgemap = threshold(template_binary_edgemap, 0.5, THRESH_ZERO, THRESH_MAX);
		SImageIO::write_png_file("image_binary_edgemap.png", temp_image_binary_edgemap, temp_image_binary_edgemap, temp_image_binary_edgemap);
		SImageIO::write_png_file("template_binary_edgemap.png", temp_template_binary_edgemap, temp_template_binary_edgemap, temp_template_binary_edgemap);
		//std::vector<DetectedSymbol> symbols1;
		//return symbols1;

		//SDoublePlane dist_score = SImageIO::read_png_file("1234.png");

		//calculating the the distance matrix D
		std::cout<<"starting distance calculator.."<<std::endl;
		SDoublePlane D_mat = dist(image_binary_edgemap);

		//reading directly from the dist_matrix image saved
		//SDoublePlane D_mat = SImageIO::read_png_file("music1_dist_matrix.png");

		//std::cout<<"writing the distance matrix of the image:"<<std::endl;
		//SImageIO::write_png_file("music1_dist_matrix.png", D_mat, D_mat, D_mat);
		//std::cout<<"done with distance"<<std::endl;

		//calculating the scores
		SDoublePlane dist_score(input.rows(),input.cols());
		std::cout<<"pushing the distance scores.."<<std::endl;
		double sum = 0;
		for(int i=0; i<input.rows(); i++)
		{
			for(int j=0; j<input.cols(); j++)
			{
				sum = 0;
				for(int k=0; k<temp.rows(); k++)
				{
					for(int l=0;l<temp.cols(); l++)
					{
						if((i+k) < input.rows() && (j+l)<input.cols())
							sum += template_binary_edgemap[k][l] * D_mat[i+k][j+l];
					}
				}
				dist_score[i][j] = sum;
			}
		}

		//std::cout<<std::endl<<"printing the values of the score matrix"<<std::endl;
		//SImageIO::write_png_file("1234.png", dist_score, dist_score, dist_score);

		SDoublePlane inverted = normalise(dist_score)*-1 + 255;
		SImageIO::write_png_file("debug.png", inverted, inverted, inverted);
		//dist_score = threshold(dist_score, 300, THRESH_MAX, THRESH_ZERO);
		std::vector<DetectedSymbol> symbols;
		for (int i=0; i<inverted.rows(); i++) {
			for (int j=0; j<inverted.cols(); j++) {
				if (inverted[i][j] < 60) {
					DetectedSymbol s;
					s.row = i-3;
					s.col = j-5;
					s.width = 25;
					s.height = 15;
					s.type = (Type)2;
					s.confidence = 0.7;
					s.pitch = 'A';
					symbols.push_back(s);
				}
			}
		}


		return symbols;
	}

}

EdgeTemplateDetector::EdgeTemplateDetector(const SDoublePlane& img): image(img) {
	
}

std::vector<DetectedSymbol> EdgeTemplateDetector::find(const SDoublePlane& input, double threshold) {
	return ::score_using_edgemaps(image);
}
