#pragma once

#include "IMetric.h"
#include <atomic>
#include <string>

class CounterMetric : public IMetric {
public:
    explicit CounterMetric(std::string name);

    const std::string& getName() const override;
    std::string getValueAndReset() override;

    // Новый метод: увеличивает счетчик, если число делится на 3
    void considerNumber(int value);

private:
    std::string _name;
    std::atomic<int> _counter{0};
};