cmake_minimum_required(VERSION 3.28.0 FATAL_ERROR)

project(ccl-project VERSION 0.0.2.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_SCAN_FOR_MODULES)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

add_subdirectory(ISL EXCLUDE_FROM_ALL SYSTEM)

include(cmake/ProjectOptions.cmake)
include(cmake/Libraries.cmake)

include(programs/CMakeLists.txt)
include(src/CMakeLists.txt)
include(catch/CMakeLists.txt)
include(bench/CMakeLists.txt)

install(DIRECTORY include/ccl ISL/include
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")

target_include_directories(ccl
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

include(CMakePackageConfigHelpers)

set_property(TARGET ccl PROPERTY VERSION ${version})
