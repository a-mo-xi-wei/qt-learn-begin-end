cmake_minimum_required(VERSION 3.10)

project(Client VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED Widgets UiTools WebSockets)

set(PROJECT_SOURCES 
	main.cpp
	RemoteFileDlg.h
	RemoteFileDlg.cpp
	Util.h
	Util.cpp
)


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})


target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::UiTools Qt6::WebSockets)
