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
#include "Gaussblur.h"

int process(const char* filename) {
	const clock_t start = clock();
	SDoublePlane input_image= SImageIO::read_png_file(filename);
	Gaussblur gauss(5);

	
	//calculating the template matching score using edgemaps method
	
	// test step 2 by applying mean filters to the input image
	SDoublePlane smoothed_image = gauss.blur(input_image);
	SImageIO::write_png_file("_smoothed.png", smoothed_image, smoothed_image, smoothed_image);


	SDoublePlane canny_image = canny(smoothed_image, 400, 0);
	SImageIO::write_png_file("edges.png", canny_image,  canny_image,  canny_image);

	std::vector<DetectedSymbol> detected;
	TemplateDetector detector1(SImageIO::read_png_file("template1.png"), NOTEHEAD);
	std::vector<DetectedSymbol> symbols1 = detector1.find(smoothed_image, 240);
	detected.insert(detected.end(), symbols1.begin(), symbols1.end());

	TemplateDetector detector2(SImageIO::read_png_file("template2.png"), QUARTERREST);
	std::vector<DetectedSymbol> symbols2 = detector2.find(smoothed_image, 240);
	detected.insert(detected.end(), symbols2.begin(), symbols2.end());

	TemplateDetector detector3(SImageIO::read_png_file("template3.png"), EIGHTHREST);
	std::vector<DetectedSymbol> symbols3 = detector3.find(smoothed_image, 240);
	detected.insert(detected.end(), symbols3.begin(), symbols3.end());

	std::vector<Line> lines = HoughLinesDetector(200).find(canny_image);
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
