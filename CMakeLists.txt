cmake_minimum_required(VERSION 3.2)

project(switchsim)

set(CMAKE_CXX_STANDARD  11)

enable_testing()


set(headers "${PROJECT_SOURCE_DIR}/support/module.hpp")
include_directories(support)

add_subdirectory(schedulers)