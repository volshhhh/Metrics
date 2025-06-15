#pragma once

#include <string>

class IMetric {
public:
  virtual ~IMetric() = default;

  virtual const std::string &getName() const = 0;

  virtual std::string getValueAndReset() = 0;
};