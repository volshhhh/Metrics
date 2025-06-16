cmake_minimum_required(VERSION 3.10)

project(MyProject LANGUAGES CXX)

add_subdirectory(ExampleMetrics)
add_subdirectory(MetricUtils)
add_subdirectory(click)

add_executable(my_app src/main.cpp)

target_link_libraries(my_app
    PRIVATE
        metrics_lib
        metric_utils_lib
        click_lib
)


target_include_directories(my_app PRIVATE
    ${CMAKE_SOURCE_DIR}/ExampleMetrics
    ${CMAKE_SOURCE_DIR}/MetricUtils
     ${CMAKE_SOURCE_DIR}/click
)