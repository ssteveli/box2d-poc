cmake_minimum_required(VERSION 3.0.0)
project(box2dballs VERSION 0.1.0)

include(CTest)
enable_testing()

link_directories(/usr/local/lib64)
find_package(box2d REQUIRED)

add_executable(box2dballs main.cpp)
target_link_libraries(${PROJECT_NAME} X11 GL pthread png stdc++fs box2d)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
