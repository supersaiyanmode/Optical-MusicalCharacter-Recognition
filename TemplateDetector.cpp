#include "TemplateDetector.h"

TemplateDetector::TemplateDetector(const SDoublePlane& kernel):
				mainKernel(kernel), invertedKernel(kernel) {
	//Write a for loop and invert every pixel and store into inverted Kernel object.
}

std::vector<DetectedSymbol> TemplateDetector::find(double threshold) {
	//use convolve_general(..) here and return a list of DetectedSymbol
}
