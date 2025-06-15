#include "MemoryUsageMetric.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

MemoryUsageMetric::MemoryUsageMetric(const std::string &name) : name_(name) {}

#ifdef __linux__
long long MemoryUsageMetric::getMemoryUsageKbLinux() {
  long long vm_rss = 0;
  std::ifstream file("/proc/self/status");
  std::string line;
  while (std::getline(file, line)) {
    if (line.rfind("VmRSS:", 0) == 0) {
      std::stringstream ss(line);
      std::string label;
      ss >> label >> vm_rss;
      break;
    }
  }
  return vm_rss;
}
#elif _WIN32
long long MemoryUsageMetric::getMemoryUsageKbWindows() {
  PROCESS_MEMORY_COUNTERS_EX pmc;
  if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc,
                           sizeof(pmc))) {
    return pmc.PrivateUsage / 1024;
  }
  return 0;
}
#elif __APPLE__
long long MemoryUsageMetric::getMemoryUsageKbMacOS() {
  mach_task_basic_info_data_t info;
  mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;

  if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info,
                &infoCount) == KERN_SUCCESS) {
    return info.resident_size / 1024;
  }
  return 0;
}
#endif

std::string MemoryUsageMetric::getValueAndReset() {
  long long memory_kb = 0;
#ifdef __linux__
  memory_kb = getMemoryUsageKbLinux();
#elif _WIN32
  memory_kb = getMemoryUsageKbWindows();
#elif __APPLE__
  memory_kb = getMemoryUsageKbMacOS();
#else
  std::cerr << "MemoryUsageMetric not supported on this platform." << std::endl;
  return "N/A";
#endif

  return std::to_string(memory_kb);
}

std::string& MemoryUsageMetric::getName() const { 
    static std::string str = name_;
    return str; 
}