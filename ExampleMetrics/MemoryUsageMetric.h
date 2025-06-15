#pragma once

#include "IMetric.h"
#include <string>
#include <atomic> 

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#elif defined(__APPLE__)
#include <mach/mach.h>
#endif

class MemoryUsageMetric : public IMetric {
public:
    explicit MemoryUsageMetric(const std::string& name = "memory_usage_mb");

    std::string getValueAndReset() override;
    std::string& getName() const override;

private:
    std::string name_;

    long long getMemoryUsageKbLinux();
    long long getMemoryUsageKbWindows();
    long long getMemoryUsageKbMacOS();
};