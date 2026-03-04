cmake_minimum_required(VERSION 3.10)
project(CoinDetector)

set(CMAKE_CXX_STANDARD 17)

# Поиск OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(CoinDetector src/main.cpp)

# Связка с библиотеками OpenCV
target_link_libraries(CoinDetector ${OpenCV_LIBS})