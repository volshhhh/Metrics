#pragma once

#include "IMetric.h"
#include <atomic>
#include <mutex>
#include <string>

class CounterMetric : public IMetric {
public:
  explicit CounterMetric(std::string name);
  const std::string &getName() const override;
  std::string getValueAndReset() override;

  void increment();

private:
  const std::string _name;
  std::atomic<uint64_t> _counter{0};
};