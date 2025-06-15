#include "MetricsWriter.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>

std::string getTimestamp() {
  auto now = std::chrono::system_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) %
            1000;
  auto timer = std::chrono::system_clock::to_time_t(now);

  std::tm bt{};
#ifdef _WIN32
  localtime_s(&bt, &timer);
#else
  localtime_r(&timer, &bt);
#endif

  std::stringstream ss;
  ss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S");
  ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
  return ss.str();
}

MetricsWriter::MetricsWriter(std::string filename,
                             std::chrono::milliseconds writeInterval)
    : _filename(std::move(filename)), _writeInterval(writeInterval) {
  std::ofstream ofs(_filename, std::ios::trunc);
}

MetricsWriter::~MetricsWriter() {
  _stopFlag = true;
  if (_writerThread.joinable()) {
    _writerThread.join();
  }
}

void MetricsWriter::registerMetric(IMetric *metric) {
  _metrics.push_back(metric);
}

void MetricsWriter::start() {
  _writerThread = std::thread(&MetricsWriter::run, this);
}

void MetricsWriter::run() {
  std::ofstream file(_filename, std::ios::app);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open metrics file: " << _filename
              << std::endl;
    return;
  }

  while (!_stopFlag) {
    std::this_thread::sleep_for(_writeInterval);

    if (_metrics.empty())
      continue;

    std::stringstream line;
    line << getTimestamp();

    for (IMetric *metric : _metrics) {
      line << " \"" << metric->getName() << "\" " << metric->getValueAndReset();
    }

    file << line.str() << std::endl;
  }
}