#include "AverageMetric.h"
#include <iomanip>
#include <sstream>

AverageMetric::AverageMetric(std::string name) : _name(std::move(name)) {}

const std::string& AverageMetric::getName() const {
    return _name;
}

std::string AverageMetric::getValueAndReset() {
    std::lock_guard<std::mutex> lock(_mutex);
    double average = 0.0;
    if (_count > 0) {
        average = _sum / _count;
    }
    
    _sum = 0.0;
    _count = 0;
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << average;
    return ss.str();
}

void AverageMetric::addSample(double value) {
    std::lock_guard<std::mutex> lock(_mutex);
    _sum += value;
    _count++;
}