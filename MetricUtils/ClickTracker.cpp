#include "ClickTracker.h"

ClickTracker::ClickTracker(int robot_threshold_k,
                           std::chrono::milliseconds robot_interval_l)
    : _robotThresholdK(robot_threshold_k), _robotIntervalL(robot_interval_l) {}

void ClickTracker::registerClick(const std::string &userId) {
  std::lock_guard<std::mutex> lock(_mutex);
  _userClicks[userId].push_back(std::chrono::steady_clock::now());
  _totalClicksSinceReset++;
}

long long ClickTracker::getTotalClicksAndReset() {
  std::lock_guard<std::mutex> lock(_mutex);
  long long currentClicks = _totalClicksSinceReset;
  _totalClicksSinceReset = 0;
  return currentClicks;
}

long long ClickTracker::getRobotCountAndReset() {
  std::lock_guard<std::mutex> lock(_mutex);
  long long robotCount = 0;
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

  for (auto it = _userClicks.begin(); it != _userClicks.end();) {
    std::string userId = it->first;
    std::deque<std::chrono::steady_clock::time_point> &clicks = it->second;

    while (!clicks.empty() && (now - clicks.front()) > _robotIntervalL) {
      clicks.pop_front();
    }

    if (clicks.size() >= static_cast<size_t>(_robotThresholdK)) {
      robotCount++;
    }

    if (clicks.empty()) {
      it = _userClicks.erase(it);
    } else {
      ++it;
    }
  }
  return robotCount;
}