#pragma once

#include <deque>
#include <unordered_map>
#include <string>
#include <chrono>
#include <mutex>
#include <unordered_set>
#include <atomic>

class ClickTracker {
private:
    std::unordered_map<std::string, std::deque<std::chrono::steady_clock::time_point>> userEvents;
    std::chrono::milliseconds robotInterval;
    mutable std::mutex mtx;

    long long clicks;
    int robotClicks;
    std::unordered_set<std::string> bannedUsers;

    std::atomic<int> newlyBannedRobotsCount;

public:
    explicit ClickTracker() = default;
    ClickTracker(std::chrono::milliseconds robotInterval_, int robotClicks_);

    void registerClick(const std::string& userId);
    void registerClick(const std::string& userId, std::chrono::steady_clock::time_point tp);

    int cntRobots();
    int cntUsers();

    int getNewlyBannedRobotsAndReset();
};