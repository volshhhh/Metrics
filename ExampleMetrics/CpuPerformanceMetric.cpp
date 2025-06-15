#include "CpuPerformanceMetric.h"
#include <cmath>

inline unsigned long long get_tsc_ticks() {
#ifdef _MSC_VER
  return __rdtsc();
#elif defined(__GNUC__) || defined(__clang__)
  return __rdtsc();
#else
  return 0;
#endif
}

CpuPerformanceMetric::CpuPerformanceMetric(
    const std::string &name, std::chrono::milliseconds calculation_interval_ms)
    : name_(name),
      ops_per_interval_count_(0),
      calculation_interval_ms_(calculation_interval_ms),
      measuring_running_(false) {
}

CpuPerformanceMetric::~CpuPerformanceMetric() {
  stopMeasuring();
  if (measuring_thread_.joinable()) {
    measuring_thread_.join();
  }
}

void CpuPerformanceMetric::startMeasuring() {
  if (!measuring_running_) {
    measuring_running_ = true;
    measuring_thread_ =
        std::thread(&CpuPerformanceMetric::measurePerformance, this);
  }
}

void CpuPerformanceMetric::stopMeasuring() { measuring_running_ = false; }

void CpuPerformanceMetric::measurePerformance() {
  long long local_ops_count = 0;
  while (measuring_running_) {

    unsigned long long start_tsc = get_tsc_ticks();
    long long current_loop_ops = 0;

    auto loop_start_time = std::chrono::high_resolution_clock::now();
    while (measuring_running_) {
      volatile double val = 1.0;
      for (int i = 0; i < 1000; ++i) {
        val = std::sqrt(val + 0.001 * i);
        val = std::sin(val) * std::cos(val);
      }
      current_loop_ops++;

      auto current_time = std::chrono::high_resolution_clock::now();
      auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                            current_time - loop_start_time)
                            .count();

      if (elapsed_ms >= calculation_interval_ms_.count()) {
        break;
      }
    }
    ops_per_interval_count_.store(current_loop_ops);
    auto actual_end_time = std::chrono::high_resolution_clock::now();
    auto actual_elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(actual_end_time -
                                                              loop_start_time)
            .count();

    if (actual_elapsed_ms < calculation_interval_ms_.count()) {
      std::this_thread::sleep_for(calculation_interval_ms_ -
                                  std::chrono::milliseconds(actual_elapsed_ms));
    }
  }
}

std::string CpuPerformanceMetric::getValueAndReset() {
  long long value = ops_per_interval_count_.exchange(0);
  return std::to_string(value);
}

std::string &CpuPerformanceMetric::getName() const {
  static std::string str = name_;
  return str;
}