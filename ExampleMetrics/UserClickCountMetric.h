#pragma once

#include "../MetricUtils/ClickTracker.h"
#include "IMetric.h"

class UserClickCountMetric : public IMetric {
public:
  UserClickCountMetric(ClickTracker *tracker);
  std::string &getName() const override;
  std::string getValueAndReset() override;

private:
  ClickTracker *_tracker;
};