add_library(metrics_lib STATIC
    CounterMetric.cpp
    CpuPerformanceMetric.cpp
    MemoryUsageMetric.cpp
)

target_include_directories(metrics_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)