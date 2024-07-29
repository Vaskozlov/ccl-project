add_executable(
        ccl-lexer
        programs/ccll.cpp
)

isl_set_project_warnings(
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

if (PRECOMPILED_HEADER)
    target_precompile_headers(
            ccl-lexer
            PUBLIC
            "pch.hpp"
    )
endif ()

target_link_libraries(
        ccl-lexer
        ccl
        fmt::fmt-header-only
        cxxopts::cxxopts
)

add_executable(
        ast-lang
        programs/ast-lang.cpp
)

isl_set_project_warnings(
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

if (PRECOMPILED_HEADER)
    target_precompile_headers(
            ast-lang
            PUBLIC
            "pch.hpp"
    )
endif ()

target_link_libraries(
        ast-lang
        ccl
        astlang
        isl
        fmt::fmt-header-only
        cxxopts::cxxopts
)