file(
        GLOB_RECURSE
        FSC_LEX_GENERATOR_OBJECTS
        "programs/*.cpp"
)

add_executable(
        ccl-lexer
        ${FSC_LEX_GENERATOR_OBJECTS}
)

target_link_libraries(
        ccl-lexer
        Boost::program_options
)

target_link_libraries(
        ccl-lexer
        fmt::fmt
)

target_link_libraries(
        ccl-lexer
        ccl
)

if (${PRECOMPILED_HEADER})
    target_precompile_headers(
            ccl-lexer
            REUSE_FROM
            ccl-catch
    )
endif ()
