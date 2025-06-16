#include "ClickTracker.h"

ClickTracker::ClickTracker(std::chrono::milliseconds robotInterval_,
                           int robotClicks_)
    : robotInterval(robotInterval_), clicks(0), robotClicks(robotClicks_),
      newlyBannedRobotsCount(0) {}

void ClickTracker::registerClick(const std::string &userId,
                                 std::chrono::steady_clock::time_point time) {
  std::lock_guard<std::mutex> lock(mtx);

  if (bannedUsers.count(userId)) {
    return;
  }

  clicks++;

  while (!userEvents[userId].empty() &&
         userEvents[userId].front() < time - robotInterval) {
    userEvents[userId].pop_front();
  }
  userEvents[userId].push_back(time);

  bool isRobot = userEvents[userId].size() >= robotClicks;
  if (isRobot) {
    if (bannedUsers.insert(userId).second) {
      newlyBannedRobotsCount.fetch_add(1, std::memory_order_relaxed);
    }
    userEvents[userId].clear();
    userEvents.erase(userId);
  }
}

void ClickTracker::registerClick(const std::string &userId) {
  auto time = std::chrono::steady_clock::now();
  registerClick(userId, time);
}

int ClickTracker::cntRobots() {
  std::lock_guard<std::mutex> lock(mtx);
  int count = bannedUsers.size();
  return count;
}

int ClickTracker::cntUsers() {
  std::lock_guard<std::mutex> lock(mtx);
  int count = userEvents.size();
  return count;
}

int ClickTracker::getNewlyBannedRobotsAndReset() {
  return newlyBannedRobotsCount.exchange(0, std::memory_order_relaxed);
}