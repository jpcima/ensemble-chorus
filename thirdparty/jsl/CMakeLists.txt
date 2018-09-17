cmake_minimum_required(VERSION 3.3)

project(jsl LANGUAGES CXX)

add_library(jsl INTERFACE)
target_include_directories(jsl INTERFACE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(jsl INTERFACE gsl-lite)
