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

isl_set_target_flags(
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

if (CCL_PRECOMPILED_HEADERS)
    target_precompile_headers(
            ccl
            PUBLIC
            "pch.hpp"
    )
endif ()

target_link_libraries(
        ccl
        PRIVATE
        fmt::fmt-header-only
)

target_link_libraries(
        ccl
        PUBLIC
        isl
)

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
