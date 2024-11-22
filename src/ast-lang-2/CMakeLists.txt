file(
        GLOB_RECURSE
        ASTLANG_OBJECTS
        "src/ast-lang-2/*.cpp"
)

if (CCL_STATIC_LIBRARY)
    add_library(astlang2 STATIC ${ASTLANG_OBJECTS})
else ()
    add_library(astlang2 SHARED ${ASTLANG_OBJECTS})
endif ()

isl_set_target_flags(
        astlang2
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
            astlang2
            REUSE_FROM
            ccl
    )
endif ()

target_link_libraries(
        astlang2
        PRIVATE
        fmt::fmt-header-only
)

target_link_libraries(
        astlang2
        PUBLIC
        ccl
)

target_include_directories(astlang2
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
