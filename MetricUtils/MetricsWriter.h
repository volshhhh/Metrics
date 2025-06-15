#pragma once

#include "../ExampleMetrics/IMetric.h"
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

class MetricsWriter {
public:
    MetricsWriter(std::string filename, std::chrono::milliseconds writeInterval);
    ~MetricsWriter();

    MetricsWriter(const MetricsWriter&) = delete;
    MetricsWriter& operator=(const MetricsWriter&) = delete;

    void registerMetric(IMetric* metric);
    void start();

private:
    void run();
    
    std::string _filename;
    std::vector<IMetric*> _metrics;
    std::thread _writerThread;
    std::atomic<bool> _stopFlag{false};
    std::chrono::milliseconds _writeInterval;
};