#ifndef __EDGE_TEMPLATE_DETECTOR_H__
#define __EDGE_TEMPLATE_DETECTOR_H__

#include <vector>

#include "Symbols.h"
#include "SImage.h"

class EdgeTemplateDetector {
	SDoublePlane image;
	SDoublePlane edge_map;
public:
	EdgeTemplateDetector(const SDoublePlane&);
	std::vector<DetectedSymbol> find(const SDoublePlane&, double threshold);
};

#endif
