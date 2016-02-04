#include "TemplateDetector.h"
#include "utils.h"

TemplateDetector::TemplateDetector(const SDoublePlane& kernel, double thresh) {
	//Write a for loop and invert every pixel and store into inverted Kernel object.
	
	mainKernel = threshold(kernel, thresh, THRESH_ZERO, THRESH_MAX);
	invertedKernel = mainKernel*-1 + 255;
}

std::vector<DetectedSymbol> TemplateDetector::find(const SDoublePlane& image, double thresh) {
	//use convolve_general(..) here and return a list of DetectedSymbol
	
	SDoublePlane output1 = convolve_general(image, mainKernel);
	SDoublePlane output2 = convolve_general(image*-1 + 255, invertedKernel);
	SDoublePlane output = threshold(output1 + output2, thresh, THRESH_ZERO, THRESH_MAX);
}
