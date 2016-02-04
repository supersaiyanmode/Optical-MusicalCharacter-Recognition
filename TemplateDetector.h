#ifndef TEMPLATE_DETECTOR_H
#define TEMPLATE_DETECTOR_H

#include <vector>

#include "SImage.h"
#include "Symbols.h"

class TemplateDetector {
	SDoublePlane mainKernel, invertedKernel;
public:
	TemplateDetector(const SDoublePlane& kernel, double);

	std::vector<DetectedSymbol> find(const SDoublePlane& image, double threshold);
};

#endif
