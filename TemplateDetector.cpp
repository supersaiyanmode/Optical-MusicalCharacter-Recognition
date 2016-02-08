#include "TemplateDetector.h"
#include "utils.h"
#include "SImageIO.h"

TemplateDetector::TemplateDetector(const SDoublePlane& kernel, Type type):type(type) {
	mainKernel = threshold(flipxy(normalise_kernel(kernel)), 195, THRESH_ZERO, THRESH_MAX);//150
	invertedKernel = mainKernel*-1 + 255;
}

std::vector<DetectedSymbol> TemplateDetector::find(const SDoublePlane& image, double thresh) {
	//SImageIO::write_png_file("_main.png", mainKernel, mainKernel, mainKernel);
	//SImageIO::write_png_file("_inverted.png", invertedKernel, invertedKernel, invertedKernel);
	SDoublePlane invertedImage = image* -1 + 255;
	//SImageIO::write_png_file("_invertedImage.png", invertedImage, invertedImage, invertedImage);
	SDoublePlane output1 = convolve_general(image, mainKernel);
	SDoublePlane output2 = convolve_general(invertedImage, invertedKernel);
	SDoublePlane output = normalise(output1 + output2);
	//SImageIO::write_png_file("out.png", output, output, output);
	output = threshold(output, thresh, THRESH_ZERO, THRESH_MAX);
	std::vector<DetectedSymbol> symbols;
	for (int i=0; i<output.rows(); i++) {
		for (int j=0; j<output.cols(); j++) {
			if (output[i][j] > 255 - 1) {
				DetectedSymbol s;
				s.row = i - mainKernel.rows()/2;
				s.col = j - mainKernel.cols()/2;
				s.width = mainKernel.cols();
				s.height = mainKernel.rows();
				s.type = type;
				s.confidence = 0.7;
				s.pitch = 'A';
				symbols.push_back(s);
			}
		}
	}
	return symbols;
}
