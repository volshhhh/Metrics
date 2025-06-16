#pragma once
#include "../ExampleMetrics/IMetric.h"
#include "ClickTracker.h"
#include <memory>

class RobotCount : public IMetric {
private:
    std::string name_;
    std::shared_ptr<ClickTracker> tracker_;

public:
    RobotCount(const std::string& name, std::shared_ptr<ClickTracker> tracker);

    const std::string &getName() const override;
    std::string getValueAndReset() override;
};