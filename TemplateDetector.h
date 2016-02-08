#ifndef TEMPLATE_DETECTOR_H
#define TEMPLATE_DETECTOR_H

#include <vector>

#include "SImage.h"
#include "Symbols.h"

class TemplateDetector {
	SDoublePlane mainKernel, invertedKernel;
	Type type;
public:
	TemplateDetector(const SDoublePlane& kernel, Type);

	std::vector<DetectedSymbol> find(const SDoublePlane& image, double threshold);
};

#endif
