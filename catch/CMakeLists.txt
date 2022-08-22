enable_testing()

file(
        GLOB_RECURSE
        LIBCERBERUS_CATCH
        "catch/*.cpp"
)

add_executable(
        libcerberus-catch
        ${LIBCERBERUS_CATCH}
)

target_compile_definitions(
        libcerberus-catch
        PRIVATE
        "BOOST_TEST_DYN_LINK"
)

set_source_files_properties(
        catch/runner.cpp PRIVATE
        PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

target_link_libraries(
        libcerberus-catch
        fmt::fmt
)

target_link_libraries(
        libcerberus-catch
        ${Boost_LIBRARIES}
)

target_link_libraries(
        libcerberus-catch
        cerberus
)

if (${PRECOMPILED_HEADER})
    target_precompile_headers(
            libcerberus-catch
            PUBLIC
            "pch.hpp"
            <boost/test/unit_test.hpp>
    )
endif ()

add_test(libcerberus-catch-test libcerberus-catch)
