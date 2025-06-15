#include "../ExampleMetrics/AverageMetric.h"
#include "../ExampleMetrics/CounterMetric.h"
#include "../ExampleMetrics/RobotDetectionMetric.h"
#include "../ExampleMetrics/UserClickCountMetric.h"

#include "../MetricUtils/ClickTracker.h"
#include "../MetricUtils/MetricsWriter.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <vector>

void sleep_ms(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
  std::random_device rd;
  std::mt19937 global_gen(rd());

  auto counter = std::make_unique<CounterMetric>("requests");
  auto average = std::make_unique<AverageMetric>("latency_ms");

  auto clickTracker =
      std::make_unique<ClickTracker>(5, std::chrono::milliseconds(1000));

  auto userClickCount =
      std::make_unique<UserClickCountMetric>(clickTracker.get());
  auto robotCount = std::make_unique<RobotDetectionMetric>(clickTracker.get());

  MetricsWriter writer("metrics.log", std::chrono::milliseconds(1000));
  writer.registerMetric(counter.get());
  writer.registerMetric(average.get());
  writer.registerMetric(userClickCount.get());
  writer.registerMetric(robotCount.get());
  writer.start();

  std::atomic<bool> running{true};

  std::thread counterThread([&] {
    std::mt19937 eng(global_gen());
    std::uniform_int_distribution<int> sleep_dist_ms(
        50, 80);
    while (running) {
      counter->increment();
      std::this_thread::sleep_for(
          std::chrono::milliseconds(sleep_dist_ms(eng)));
    }
  });

  std::thread averageThread([&] {
    std::mt19937 eng(global_gen());
    std::uniform_real_distribution<double> latency_dist(10.0, 500.0);
    std::uniform_int_distribution<int> sleep_dist_ms(100, 300);
    while (running) {
      average->addSample(latency_dist(eng));
      std::this_thread::sleep_for(
          std::chrono::milliseconds(sleep_dist_ms(eng)));
    }
  });

  std::thread clickSimulationThread([&] {
    std::mt19937 eng(global_gen());
    std::uniform_int_distribution<> user_id_dist(1, 20);
    std::uniform_int_distribution<int> clicks_per_tick_dist(1, 5);

    std::uniform_int_distribution<int> num_robots_to_emulate_dist(1, 3);

    std::uniform_int_distribution<int> robot_click_delay_ms(1, 50);

    std::uniform_int_distribution<int> robot_clicks_count_dist(6, 15);

    std::vector<std::string> robot_ids_to_activate;
    int current_robot_index = 0;

    int desired_robot_count_for_simulation = num_robots_to_emulate_dist(eng);
    std::cout << "Targeting " << desired_robot_count_for_simulation
              << " robots to be active during simulation." << std::endl;

    for (int i = 0; i < desired_robot_count_for_simulation; ++i) {
      robot_ids_to_activate.push_back("robot_" +
                                      std::to_string(user_id_dist(eng)));
    }

    int seconds_elapsed = 0;
    while (running) {
      int clicks_this_tick = clicks_per_tick_dist(eng);
      for (int j = 0; j < clicks_this_tick; ++j) {
        std::string userId = "user_" + std::to_string(user_id_dist(eng));
        clickTracker->registerClick(userId);
      }

      if (current_robot_index < desired_robot_count_for_simulation &&
          (seconds_elapsed % 2 == 0 || seconds_elapsed % 3 == 0) &&
          seconds_elapsed > 0) {
        std::string current_robot_id =
            robot_ids_to_activate[current_robot_index];
        int num_robot_clicks = robot_clicks_count_dist(eng);

        std::cout << "Simulating robot activity for: " << current_robot_id
                  << " (" << num_robot_clicks << " clicks)" << std::endl;
        for (int k = 0; k < num_robot_clicks; ++k) {
          clickTracker->registerClick(current_robot_id);
          sleep_ms(robot_click_delay_ms(eng));
        }
        current_robot_index++;
      }

      seconds_elapsed++;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });

  std::cout << "Metrics collection running for 10 seconds..." << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(10));
  running = false;

  counterThread.join();
  averageThread.join();
  clickSimulationThread.join();

  std::cout << "Done. Metrics written to metrics.log" << std::endl;
  return 0;
}