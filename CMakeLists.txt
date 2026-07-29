cmake_minimum_required(VERSION 3.16)
project(myschedule VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 开启 Qt AUTOMOC / AUTOUIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Threads REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(include)

# 显式收集 src 目录下的 .cpp 和 include 目录下的 .h
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# 将头文件也一并加入 target，这样 CMake 的 AUTOMOC 就能精准扫描 Q_OBJECT
add_executable(myschedule ${SOURCES} ${HEADERS})

target_link_libraries(myschedule PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Threads::Threads)
