cmake_minimum_required(VERSION 3.14)

project(LogOut LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

#添加宏定义  定义此宏就能够在Release模式下的日志输出 输出函数名...
add_compile_definitions(QT_MESSAGELOGCONTEXT)
# 无法关闭错误
#add_compile_definitions(QT_NO_DEBUG_OUTPUT QT_NO_INFO_OUTPUT QT_NO_WARNING_OUTPUT QT_NO_CRITICAL_OUTPUT)


add_executable(LogOut
  main1.cpp
)


target_link_libraries(LogOut Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS LogOut
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
