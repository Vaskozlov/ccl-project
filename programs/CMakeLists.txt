file(
        GLOB_RECURSE
        FSC_LEX_GENERATOR_OBJECTS
        "programs/*.cpp"
)

find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(
        ccll
        ${FSC_LEX_GENERATOR_OBJECTS}
)

target_link_libraries(
        ccll
        Boost::program_options
)

target_link_libraries(
        ccll
        fmt::fmt
)

target_link_libraries(
        ccll
        ccl
)
