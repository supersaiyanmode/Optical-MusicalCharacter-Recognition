#include <cmath>
#include <iostream>

#include "DrawText.h"
#include "SImage.h"
#include "SImageIO.h"
#include "TemplateDetector.h"
#include "Kernel.h"
#include "Hough.h"
#include "Canny.h"
#include "utils.h"
#include "detection_utils.h"

using namespace std;

// The simple image class is called SDoublePlane, with each pixel represented as
// a double (floating point) type. This means that an SDoublePlane can represent
// values outside the range 0-255, and thus can represent squared gradient magnitudes,
// harris corner scores, etc. 
//
// The SImageIO class supports reading and writing PNG files. It will read in
// a color PNG file, convert it to grayscale, and then return it to you in 
// an SDoublePlane. The values in this SDoublePlane will be in the range [0,255].
//
// To write out an image, call write_png_file(). It takes three separate planes,
// one for each primary color (red, green, blue). To write a grayscale image,
// just pass the same SDoublePlane for all 3 planes. In order to get sensible
// results, the values in the SDoublePlane should be in the range [0,255].
//

// Below is a helper functions that overlays rectangles
// on an image plane for visualization purpose. 

// Draws a rectangle on an image plane, using the specified gray level value and line width.
//
void overlay_rectangle(
		SDoublePlane &input,
		int _top, int _left, int _bottom, int _right,
		double graylevel, int width)
{
	for(int w=-width/2; w<=width/2; w++) {
		int top = _top+w, 
			left = _left+w,
			right=_right+w,
			bottom=_bottom+w;

		// if any of the coordinates are out-of-bounds, truncate them 
		top = min( max( top, 0 ), input.rows()-1);
		bottom = min( max( bottom, 0 ), input.rows()-1);
		left = min( max( left, 0 ), input.cols()-1);
		right = min( max( right, 0 ), input.cols()-1);

		// draw top and bottom lines
		for(int j=left; j<=right; j++)
			input[top][j] = input[bottom][j] = graylevel;
		// draw left and right lines
		for(int i=top; i<=bottom; i++)
			input[i][left] = input[i][right] = graylevel;
	}
}

// DetectedSymbol class may be helpful!
//  Feel free to modify.
//
// Function that outputs the ascii detection output file
void  write_detection_txt(const string &filename, const vector<struct DetectedSymbol> &symbols)
{
	ofstream ofs(filename.c_str());

	for(size_t i=0; i<symbols.size(); i++)
	{
		const DetectedSymbol &s = symbols[i];
		ofs << s.row << " " << s.col << " " << s.width << " " << s.height << " ";
		if(s.type == NOTEHEAD)
			ofs << "filled_note " << s.pitch;
		else if(s.type == EIGHTHREST)
			ofs << "eighth_rest _";
		else 
			ofs << "quarter_rest _";
		ofs << " " << s.confidence << endl;
	}
}

// Function that outputs a visualization of detected symbols
void  write_detection_image(const string &filename, const vector<DetectedSymbol> &symbols,
		const SDoublePlane &input)
{
	SDoublePlane output_planes[3];
	for(int i=0; i<3; i++)
		output_planes[i] = input;

	for(size_t i=0; i<symbols.size(); i++)
	{
		const DetectedSymbol &s = symbols[i];

		overlay_rectangle(output_planes[s.type], s.row, s.col, s.row+s.height-1, s.col+s.width-1, 255, 2);
		overlay_rectangle(output_planes[(s.type+1) % 3], s.row, s.col, s.row+s.height-1, s.col+s.width-1, 0, 2);
		overlay_rectangle(output_planes[(s.type+2) % 3], s.row, s.col, s.row+s.height-1, s.col+s.width-1, 0, 2);

		if(s.type == NOTEHEAD)
		{
			char str[] = {s.pitch, 0};
			draw_text(output_planes[0], str, s.row, s.col+s.width+1, 0, 2);
			draw_text(output_planes[1], str, s.row, s.col+s.width+1, 0, 2);
			draw_text(output_planes[2], str, s.row, s.col+s.width+1, 0, 2);
		}
	}

	SImageIO::write_png_file(filename.c_str(), output_planes[0], output_planes[1], output_planes[2]);
}



// The rest of these functions are incomplete. These are just suggestions to 
// get you started -- feel free to add extra functions, change function
// parameters, etc.

// Convolve an image with a separable convolution kernel
//
SDoublePlane convolve_separable1(const SDoublePlane &input, const SDoublePlane &row_filter, const SDoublePlane &col_filter)
{
	SDoublePlane output(input.rows(), input.cols());

	return output;
}


// Apply a sobel operator to an image, returns the result
// 
SDoublePlane sobel_gradient_filter(const SDoublePlane &input, bool _gx)
{
	SDoublePlane output(input.rows(), input.cols());

	// Implement a sobel gradient estimation filter with 1-d filters

	return output;
}

// Apply an edge detector to an image, returns the binary edge map
// 
SDoublePlane find_edges(const SDoublePlane &input, double thresh=0)
{
	SDoublePlane output(input.rows(), input.cols());

	// Implement an edge detector of your choice, e.g.
	// use your sobel gradient operator to compute the gradient magnitude and threshold

	return output;
}


int process(const char* filename) {
	const clock_t start = clock();
	SDoublePlane input_image= SImageIO::read_png_file(filename);

	
	//calculating the template matching score using edgemaps method
	
	// test step 2 by applying mean filters to the input image
	SDoublePlane output_image = convolve_general(input_image, load_kernel("kernels/gauss55"));
	SImageIO::write_png_file("_smoothed.png", output_image, output_image, output_image);


	output_image = canny(output_image, 400, 0);
	SImageIO::write_png_file("edges.png", output_image, output_image, output_image);

	std::vector<DetectedSymbol> detected;
	TemplateDetector detector1(SImageIO::read_png_file("template1.png"), NOTEHEAD);
	std::vector<DetectedSymbol> symbols1 = detector1.find(input_image, 240);
	detected.insert(detected.end(), symbols1.begin(), symbols1.end());

	TemplateDetector detector2(SImageIO::read_png_file("template2.png"), QUARTERREST);
	std::vector<DetectedSymbol> symbols2 = detector2.find(input_image, 240);
	detected.insert(detected.end(), symbols2.begin(), symbols2.end());

	TemplateDetector detector3(SImageIO::read_png_file("template3.png"), EIGHTHREST);
	std::vector<DetectedSymbol> symbols3 = detector3.find(input_image, 240);
	detected.insert(detected.end(), symbols3.begin(), symbols3.end());

	std::vector<Line> lines = HoughLinesDetector(200).find(output_image);
	write_detection_txt("detected.txt", detected);
	write_detection_image("detected.png", detected, input_image);
	std::cout<<"Time taken: "<<float( clock() - start)/CLOCKS_PER_SEC<<std::endl;
	return 0;
}

//
// This main file just outputs a few test images. You'll want to change it to do 
//  something more interesting!
//
int main(int argc, char *argv[])
{
	if(!(argc == 2))
	{
		std::cerr << "Usage: " << argv[0] << " input_image" << std::endl;
		return 1;
	}
	try {
		return process(argv[1]);
	} catch (const char* err) {
		std::cerr<<err<<std::endl;
		return 1;
	}
}
