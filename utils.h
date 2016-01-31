#ifndef UTILS_H
#define UTILS_H

#include "SImage.h"

SDoublePlane convolve_general(const SDoublePlane& input, const SDoublePlane& filter);
SDoublePlane convolve_separable(const SDoublePlane &input, const SDoublePlane &row_filter, const SDoublePlane &col_filter);

#endif
