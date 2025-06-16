#include "RobotCount.h"
#include "../MetricUtils/MetricsWriter.h"
#include "ClickTracker.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>

void sleep_ms(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
  const int numUsers = 15;
  const int totalClicks = 500;
  const int robotClicks = 5;
  const std::chrono::milliseconds robotInterval =
      std::chrono::milliseconds(100);

  const std::chrono::milliseconds writeInterval =
      std::chrono::milliseconds(1000);

  auto tracker = std::make_shared<ClickTracker>(robotInterval, robotClicks);
  RobotCount robotMetric("robots_detected_per_sec", tracker);

  std::vector<std::string> users;
  users.reserve(numUsers);
  for (int i = 0; i < numUsers; ++i) {
    users.push_back("user_" + std::to_string(i));
  }

  MetricsWriter writer("robot_metric_test.log", writeInterval);
  writer.registerMetric(&robotMetric);
  writer.start();

  std::atomic<int> clicksGenerated{0};

  std::thread clickThread([&] {
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> userDist(0, numUsers - 1);
    std::uniform_int_distribution<int> batchDist(1, numUsers);
    std::uniform_int_distribution<int> sleepDist(40, 60);

    while (clicksGenerated.load() < totalClicks) {
      int remaining = totalClicks - clicksGenerated.load();

      int batchSize = std::min(batchDist(rng), remaining);

      for (int i = 0; i < batchSize; ++i) {
        const auto &user = users[userDist(rng)];
        tracker->registerClick(user);
        clicksGenerated.fetch_add(1);
      }
      sleep_ms(sleepDist(rng));
    }
    std::cout << "Click generation finished. Total clicks: "
              << clicksGenerated.load() << std::endl;
  });

  clickThread.join();

  std::cout << "All clicks generated. Waiting for final metric logs..."
            << std::endl;
  std::this_thread::sleep_for(
      std::chrono::milliseconds(writeInterval.count() * 2));

  std::cout << "Simulation complete. Check 'robot_metric_test.log' for results."
            << std::endl;
}