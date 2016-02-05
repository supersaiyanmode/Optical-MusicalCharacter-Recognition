#ifndef UTILS_H
#define UTILS_H

#include "SImage.h"

SDoublePlane convolve_general(const SDoublePlane& input, const SDoublePlane& filter);
SDoublePlane convolve_separable(const SDoublePlane &input, const SDoublePlane &row_filter, const SDoublePlane &col_filter);

SDoublePlane threshold(const SDoublePlane& image, double val, int low_val, int high_val);

SDoublePlane normalise(SDoublePlane input);
#define THRESH_RETAIN -128
#define THRESH_ZERO -256
#define THRESH_MAX -512

#endif
