#include "../ExampleMetrics/CounterMetric.h"
#include "../ExampleMetrics/CpuPerformanceMetric.h"
#include "../ExampleMetrics/MemoryUsageMetric.h"
#include "../MetricUtils/MetricsWriter.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

void sleep_ms(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
  auto cpuPerformance = std::make_unique<CpuPerformanceMetric>(
      "cpu_performance_score", std::chrono::milliseconds(100));
  cpuPerformance->startMeasuring();

  auto memoryUsage = std::make_unique<MemoryUsageMetric>("memory_usage_mb");
  auto counter = std::make_unique<CounterMetric>("divisible_by_3");

  MetricsWriter writer("system_metrics.log", std::chrono::milliseconds(1000));

  writer.registerMetric(cpuPerformance.get());
  writer.registerMetric(memoryUsage.get());
  writer.registerMetric(counter.get());

  writer.start();

  std::atomic<bool> running{true};
  std::thread generatorThread([&] {
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(1, 1000);
    while (running) {
      int value = dist(rng);
      counter->considerNumber(value);
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  });

  std::cout
      << "Генерируем числа и считаем, сколько из них делится на 3 (10 секунд). "
         "Смотрите system_metrics.log"
      << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(10));
  running = false;
  generatorThread.join();

  std::cout << "Остановка измерения..." << std::endl;
}