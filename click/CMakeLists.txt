add_library(click_lib
    ClickTracker.cpp
    RobotCount.cpp
)

target_include_directories(click_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/ExampleMetrics
     ${CMAKE_SOURCE_DIR}/MetricUtils
)

add_executable(RobotCountLogTest
    RobotCountTest.cpp
)

target_link_libraries(RobotCountLogTest PRIVATE click_lib)

target_link_libraries(RobotCountLogTest
    PRIVATE
        # Вам нужны реализации методов ClickTracker и MetricsWriter
        metric_utils_lib    # <--- Убедитесь, что ЭТА СТРОКА ЕСТЬ
        # Вам нужны реализации методов RobotCount
        metrics_lib         # <--- Убедитесь, что ЭТА СТРОКА ЕСТЬ
)