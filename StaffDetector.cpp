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
    std::sort(diff_offsets.begin(), diff_offsets.end());

	//Average the smallest 5 diff_offsets to get staff height;
    _staff_height = std::accumulate(diff_offsets.begin(), diff_offsets.begin() + 5, 0) / 5.0;

	//Average the largest 5 diff_offsets to get group heights;
	double avg_group_height = std::accumulate(diff_offsets.end() - 5, diff_offsets.end(), 0);

	for (int i=0; i < diff_offsets.size();) {
		std::vector<int> curGroup;
		curGroup.push_back(lines[i]);
		while (++i < diff_offsets.size() && diff_offsets[i] < 3 * _staff_height) {
			curGroup.push_back(lines[i]);
		}
		_groups.push_back(curGroup);
	}
}

int StaffDetector::staff_height() {
	return _staff_height;
}

const std::vector<std::vector<int> >& StaffDetector::groups() {
	return _groups;
}
