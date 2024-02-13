file(
        GLOB_RECURSE
        FSC_LEX_GENERATOR_OBJECTS
        "programs/*.cpp"
)

add_executable(
        ccl-lexer
        ${FSC_LEX_GENERATOR_OBJECTS}
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
        fmt::fmt
        cxxopts::cxxopts
)
