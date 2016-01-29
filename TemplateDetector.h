#ifndef TEMPLATE_DETECTOR_H
#define TEMPLATE_DETECTOR_H


#include "SImage.h"
#include "Symbols.h"

class TemplateDetector {
	SDoublePlane mainKernel, invertedKernel;
public:
	TemplateDetector(const SDoublePlane& kernel);

	std::vector<DetectedSymbol> find(double threshold);
}

#endif
