cmake_minimum_required(VERSION 3.20.0)

project(ccl-project VERSION 0.0.1.0 LANGUAGES CXX)

include(cmake/project-options.cmake)
include(cmake/compile-options.cmake)
include(cmake/libraries.cmake)

include(src/CMakeLists.txt)
include(catch/CMakeLists.txt)
include(bench/CMakeLists.txt)
