#pragma once

#include <string>
#include <map>
#include <deque>
#include <chrono>
#include <mutex>

struct ClickEvent {
    std::string userId;
    std::chrono::steady_clock::time_point timestamp;
};

class ClickTracker {
public:
    ClickTracker(int robot_threshold_k, std::chrono::milliseconds robot_interval_l);

    void registerClick(const std::string& userId);

    long long getTotalClicksAndReset();

    long long getRobotCountAndReset();

private:
    std::map<std::string, std::deque<std::chrono::steady_clock::time_point>> _userClicks;
    mutable std::mutex _mutex;
    long long _totalClicksSinceReset = 0;

    const int _robotThresholdK;
    const std::chrono::milliseconds _robotIntervalL;
};