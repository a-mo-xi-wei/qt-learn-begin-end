cmake_minimum_required(VERSION 3.10)

project(UiTool VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED Widgets)

aux_source_directory(./ UITOOLS_FILES)

add_library(${PROJECT_NAME} STATIC ${UITOOLS_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

