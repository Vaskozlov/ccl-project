file(
        GLOB_RECURSE
        FSC_LEX_GENERATOR_OBJECTS
        "programs/fsc_lex_generator.cpp"
)

add_executable(
        fsc_lex_generator
        ${FSC_LEX_GENERATOR_OBJECTS}
)

target_link_libraries(
        fsc_lex_generator
        fmt::fmt
)

target_link_libraries(
        fsc_lex_generator
        ccl
)
