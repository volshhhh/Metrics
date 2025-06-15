#pragma once

#include "IMetric.h"
#include <atomic>
#include <chrono>
#include <string>
#include <thread>

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <x86intrin.h>
#else
#warning "RDTSC intrinsic might not be available on this compiler/platform."
#endif

class CpuPerformanceMetric: public IMetric {
public:
  explicit CpuPerformanceMetric(
      const std::string &name,
      std::chrono::milliseconds calculation_interval_ms =
          std::chrono::milliseconds(100));
  ~CpuPerformanceMetric() override;

  std::string getValueAndReset() override;
  std::string &getName() const override;

  void startMeasuring();
  void stopMeasuring();

  std::thread measuring_thread_;

private:
  std::string name_;
  std::atomic<long long> ops_per_interval_count_;
  std::chrono::milliseconds calculation_interval_ms_;
  std::atomic<bool> measuring_running_;

  void measurePerformance();
};