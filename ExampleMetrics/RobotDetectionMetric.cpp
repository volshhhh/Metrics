#include "RobotDetectionMetric.h"
#include <string>

RobotDetectionMetric::RobotDetectionMetric(ClickTracker* tracker) : _tracker(tracker) {}

std::string& RobotDetectionMetric::getName() const {
    static std::string name = "RobotCount";
    return name;
}

std::string RobotDetectionMetric::getValueAndReset() {
    if (_tracker) {
        long long count = _tracker->getRobotCountAndReset();
        return std::to_string(count);
    }
    return "Error: No tracker";
}