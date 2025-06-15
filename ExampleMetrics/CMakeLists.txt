add_library(metrics_lib STATIC
    AverageMetric.cpp
    CounterMetric.cpp
    UserClickCountMetric.cpp
    RobotDetectionMetric.cpp
)

target_include_directories(metrics_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)