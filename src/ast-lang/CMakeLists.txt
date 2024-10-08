file(
        GLOB_RECURSE
        ASTLANG_OBJECTS
        "src/ast-lang/*.cpp"
)

if (CCL_STATIC_LIBRARY)
    add_library(astlang STATIC ${ASTLANG_OBJECTS})
else ()
    add_library(astlang SHARED ${ASTLANG_OBJECTS})
endif ()

isl_set_target_flags(
        astlang
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
            astlang
            REUSE_FROM
            ccl
    )
endif ()

add_dependencies(
        astlang
        ccl
        isl
        fmt::fmt-header-only
        unordered_dense::unordered_dense
)

target_link_libraries(
        astlang
        ccl
        isl
        fmt::fmt-header-only
        unordered_dense::unordered_dense
)

target_include_directories(astlang
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
