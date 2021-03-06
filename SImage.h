#ifndef __SIMAGE_H__
#define __SIMAGE_H__

#include <string.h>

#include "DTwoDimArray.h"

// A very simple image class.
//
class SDoublePlane : public _DTwoDimArray<double>
{
public:
	SDoublePlane() { }
	SDoublePlane(int _rows, int _cols)  : _DTwoDimArray<double>(_rows, _cols) { 
		// be nice and initialize plane to all 0's
		memset(data_ptr(), 0, sizeof(double) * rows() * cols());
	}

	
	SDoublePlane& operator*=(double val);

	SDoublePlane& operator*=(const SDoublePlane& other);

	SDoublePlane& operator+=(double val);

	SDoublePlane& operator+=(const SDoublePlane& other);

};


SDoublePlane operator*(SDoublePlane one, const SDoublePlane& two);

SDoublePlane operator+(SDoublePlane one, const SDoublePlane& two);

SDoublePlane operator*(SDoublePlane one, double two);


SDoublePlane operator+(SDoublePlane one, double two);
#endif
