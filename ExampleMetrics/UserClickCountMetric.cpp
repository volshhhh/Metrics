#include "UserClickCountMetric.h"
#include <string>

UserClickCountMetric::UserClickCountMetric(ClickTracker* tracker) : _tracker(tracker) {}

std::string& UserClickCountMetric::getName() const {
    static std::string name = "TotalUserClicks";
    return name;
}

std::string UserClickCountMetric::getValueAndReset() {
    if (_tracker) {
        long long count = _tracker->getTotalClicksAndReset();
        return std::to_string(count);
    }
    return "Error: No tracker";
}