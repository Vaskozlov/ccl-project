add_executable(
        ccl-lexer
        programs/ccll.cpp
)

isl_set_target_flags(
        ccl-lexer
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
            ccl-lexer
            REUSE_FROM
            ccl
    )
endif ()

target_link_libraries(
        ccl-lexer
        PRIVATE
        fmt::fmt-header-only
)

target_link_libraries(
        ccl-lexer
        PUBLIC
        ccl
        cxxopts::cxxopts
)

add_executable(
        ast-lang
        programs/ast-lang.cpp
)

isl_set_target_flags(
        ast-lang
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
            ast-lang
            REUSE_FROM
            ccl
    )
endif ()

target_link_libraries(
        ast-lang
        PRIVATE
        fmt::fmt-header-only
)

target_link_libraries(
        ast-lang
        PUBLIC
        astlang2
        cxxopts::cxxopts
)
