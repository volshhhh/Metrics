#include "RobotCount.h"

RobotCount::RobotCount(const std::string &name,
                       std::shared_ptr<ClickTracker> tracker)
    : name_(std::move(name)), tracker_(std::move(tracker)) {}

const std::string &RobotCount::getName() const { return name_; }

std::string RobotCount::getValueAndReset() {
  return std::to_string(tracker_->getNewlyBannedRobotsAndReset());
}