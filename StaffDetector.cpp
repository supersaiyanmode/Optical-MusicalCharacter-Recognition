#include <algorithm>
#include <numeric>

#include "StaffDetector.h"

StaffDetector::StaffDetector(const std::vector<int>& offsets) {
	std::vector<int> lines(offsets);
	std::sort(lines.begin(), lines.end());

	std::vector<int> diff_offsets;
    for (std::vector<int>::iterator it = lines.begin() + 1; it != lines.end(); it++) {
        diff_offsets.push_back(std::abs(*it - *(it - 1)));
    }

	//Average the smallest 5 diff_offsets to get staff height;
    _staff_height = std::accumulate(diff_offsets.begin(), diff_offsets.begin() + 5, 0) / 5.0;

	for (int i=1; i <= diff_offsets.size();) {
		std::vector<int> curGroup;
		curGroup.push_back(lines[i-1]);
		while (++i < diff_offsets.size()) {
			curGroup.push_back(lines[i-1]);
			if (diff_offsets[i] > 3 * _staff_height) {
				break;
			}
		}
		_groups.push_back(curGroup);
	}
}

int StaffDetector::staff_height() const {
	return _staff_height;
}

const std::vector<std::vector<int> >& StaffDetector::groups() const {
	return _groups;
}
