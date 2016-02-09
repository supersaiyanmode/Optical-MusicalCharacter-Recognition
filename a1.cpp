#include <cmath>
#include <iostream>

#include "A1Debug.h"
#include "DrawText.h"
#include "SImage.h"
#include "SImageIO.h"
#include "TemplateDetector.h"
#include "EdgeTemplateDetector.h"
#include "Kernel.h"
#include "Hough.h"
#include "Canny.h"
#include "detection_utils.h"
#include "Gaussblur.h"
#include "Config.h"

int process(const char* filename) {
	Config config("config.txt");
	const clock_t start = clock();
	SDoublePlane input_image= SImageIO::read_png_file(filename);
	Gaussblur gauss(5);

	SDoublePlane smoothed_image = gauss.blur(input_image);
	debug_png("gauss.png", smoothed_image);
	
		// test step 2 by applying mean filters to the input image
	SDoublePlane canny_image = canny(smoothed_image,
						config.get<double>("canny.low_thresh"), 
						config.get<double>("canny.high_thresh"));
	debug_png("canny.png", canny_image);

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
	
	//calculating the template matching score using edgemaps method
	std::vector<DetectedSymbol> symbols4 = EdgeTemplateDetector(input_image).find(SDoublePlane(),0);
	detected.insert(detected.end(), symbols4.begin(), symbols4.end());

	HoughLinesDetector hough(config);
	std::vector<int> lines = hough.find(canny_image);
	for (std::vector<int>::iterator it=lines.begin(); it  != lines.end(); it++) {
		draw_line(input_image, input_image, input_image, *it, 0, *it, input_image.cols(), 100, 100, 100);
	}

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
