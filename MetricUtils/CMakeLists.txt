add_library(metric_utils_lib STATIC
    MetricWriter.cpp
)

target_include_directories(metric_utils_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(metric_utils_lib PRIVATE
    ${CMAKE_SOURCE_DIR}/ExampleMetrics
)