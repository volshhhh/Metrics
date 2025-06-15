#pragma once

#include "IMetric.h"
#include "../MetricUtils/ClickTracker.h"

class RobotDetectionMetric : public IMetric {
public:
    RobotDetectionMetric(ClickTracker* tracker);
    std::string& getName() const override;
    std::string getValueAndReset() override;

private:
    ClickTracker* _tracker;
};