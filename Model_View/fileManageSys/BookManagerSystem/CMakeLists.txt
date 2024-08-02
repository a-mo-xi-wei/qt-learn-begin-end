cmake_minimum_required(VERSION 3.10)

project(BookManagerSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED Widgets)

aux_source_directory(./ SOURCE_FILES)

#添加子目录
add_subdirectory(./UiTool)
add_subdirectory(./Cool_Login_Interface)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Cool_Login_Interface)