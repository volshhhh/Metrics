#include "CounterMetric.h"

CounterMetric::CounterMetric(std::string name) : _name(std::move(name)) {}

const std::string& CounterMetric::getName() const {
    return _name;
}

std::string CounterMetric::getValueAndReset() {
    return std::to_string(_counter.exchange(0));
}

void CounterMetric::considerNumber(int value) {
    if (value % 3 == 0) {
        _counter++;
    }
}