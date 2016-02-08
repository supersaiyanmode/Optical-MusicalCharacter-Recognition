#ifndef UTILS_H
#define UTILS_H

#include "SImage.h"

SDoublePlane convolve_general(const SDoublePlane& input, const SDoublePlane& filter);
SDoublePlane convolve_separable(const SDoublePlane &input, const SDoublePlane &row_filter, const SDoublePlane &col_filter);

SDoublePlane threshold(const SDoublePlane& image, double val, int low_val, int high_val);

SDoublePlane normalise(SDoublePlane input);

SDoublePlane flipxy(SDoublePlane input);

SDoublePlane normalise_kernel(const SDoublePlane& input);

SDoublePlane score_using_edgemaps(const SDoublePlane& input);
SDoublePlane convert_to_binary_edgemap(SDoublePlane input);
SDoublePlane dist(SDoublePlane img);
double f_gamma(double val);

double image_max(const SDoublePlane&);
double image_sum(const SDoublePlane&);

#define THRESH_RETAIN -128
#define THRESH_ZERO -256
#define THRESH_MAX -512

#endif
