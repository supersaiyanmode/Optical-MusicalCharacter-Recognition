#include "TemplateDetector.h"
#include "utils.h"
#include "SImageIO.h"

TemplateDetector::TemplateDetector(const SDoublePlane& kernel) {
	//Write a for loop and invert every pixel and store into inverted Kernel object.
	mainKernel = threshold(flipxy(kernel), 195, THRESH_ZERO, THRESH_MAX);//150
	invertedKernel = mainKernel*-1 + 255;
}

std::vector<DetectedSymbol> TemplateDetector::find(const SDoublePlane& image, double thresh) {
	//use convolve_general(..) here and return a list of DetectedSymbol
	
	SImageIO::write_png_file("_main.png", mainKernel, mainKernel, mainKernel);
	SImageIO::write_png_file("_inverted.png", invertedKernel, invertedKernel, invertedKernel);
	SDoublePlane invertedImage = image* -1 + 255;
	SImageIO::write_png_file("_invertedImage.png", invertedImage, invertedImage, invertedImage);
	SDoublePlane output1 = convolve_general(image, mainKernel);
	SDoublePlane output2 = convolve_general(invertedImage, invertedKernel);
	SDoublePlane output = normalise(output1 + output2);
	output = threshold(output, thresh, THRESH_ZERO, THRESH_MAX);
	SImageIO::write_png_file("out.png", output, output, output);
	std::vector<DetectedSymbol> symbols;
	for (int i=0; i<output.rows(); i++) {
		for (int j=0; j<output.cols(); j++) {
			if (output[i][j] > 255 - 1) {
				DetectedSymbol s;
				s.row = i-10;
				s.col = j-10;
				s.width = 20;
				s.height = 20;
				s.type = (Type)2;
				s.confidence = 0.7;
				s.pitch = 'A';
				symbols.push_back(s);
			}
		}
	}
	return symbols;
}
