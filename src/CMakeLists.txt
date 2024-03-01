add_library(ccl)

file(
        GLOB_RECURSE
        CCL_OBJECTS
        "include/*.ixx"
        "src/*.cxxm"
)

target_sources(
        ccl
        PUBLIC
        FILE_SET CXX_MODULES FILES
        ${CCL_OBJECTS}
)

isl_set_project_warnings(
        ccl
        ${CCL_STRICT_COMPILATION}
        ""
        ""
        ""
        ""
        ${CCL_HARDENING}
        ${CCL_LIBCPP}
        OFF
)

add_dependencies(ccl isl fmt::fmt-header-only)

target_link_libraries(
        ccl
        isl
        fmt::fmt-header-only
)

target_include_directories(ccl
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
