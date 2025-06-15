#pragma once

#include "IMetric.h"
#include <atomic>
#include <mutex>
#include <string>

class AverageMetric : public IMetric {
public:
  explicit AverageMetric(std::string name);
  const std::string &getName() const override;
  std::string getValueAndReset() override;

  void addSample(double value);

private:
  const std::string _name;
  double _sum = 0.0;
  uint64_t _count = 0;
  std::mutex _mutex;
};