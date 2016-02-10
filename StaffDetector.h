#ifndef __STAFF_DETECTOR_H
#define __STAFF_DETECTOR_H

#include <vector>

class StaffDetector {
	int _staff_height;
	int _group_count;
public:
	StaffDetector(const std::vector<int>&);

	int group_count();
	int last_staff(int);

	int staff_height();
};

#endif
