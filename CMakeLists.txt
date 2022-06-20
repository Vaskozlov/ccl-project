cmake_minimum_required(VERSION 3.20.0)

project(cerberus-project VERSION 0.0.1.0 LANGUAGES CXX)

include(cmake/.cmake-options)
include(cmake/.cmake-rules)
include(cmake/.cmake-libraries)

include(catch/CMakeLists.txt)
