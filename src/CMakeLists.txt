file(
        GLOB_RECURSE
        CCL_OBJECTS
        "src/ccl/*.cpp"
)

if (CCL_STATIC_LIBRARY)
    add_library(ccl STATIC ${CCL_OBJECTS})
else ()
    add_library(ccl SHARED ${CCL_OBJECTS})
endif ()

isl_set_project_warnings(
        ccl
        ${CCL_STRICT_COMPILATION}
        ""
        ""
        ""
        ""
        ${CCL_HARDENING}
        ${CCL_LIBCPP}
        ${CCL_UNITY_BUILD}
)

if (CCL_LIBCPP AND LINUX)
    add_dependencies(ccl fmt::fmt-header-only)

    target_link_libraries(
            ccl
            fmt::fmt-header-only
    )
else ()
    add_dependencies(ccl fmt::fmt)

    target_link_libraries(
            ccl
            fmt::fmt
    )
endif ()

if (CCL_PRECOMPILED_HEADERS)
    target_precompile_headers(
            ccl
            PUBLIC
            "pch.hpp"
    )
endif ()

target_include_directories(ccl
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
