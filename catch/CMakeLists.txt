enable_testing()

file(
        GLOB_RECURSE
        CCL_CATCH_OBJECTS
        "catch/*.cpp"
)

add_executable(
        ccl-catch
        ${CCL_CATCH_OBJECTS}
)

target_compile_definitions(
        ccl-catch
        PRIVATE
        "BOOST_TEST_DYN_LINK"
)

set_source_files_properties(
        catch/runner.cpp PRIVATE
        PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

target_link_libraries(
        ccl-catch
        fmt::fmt
)

target_link_libraries(
        ccl-catch
        ccl
)

target_link_libraries(
        ccl-catch
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

if (${PRECOMPILED_HEADER})
    target_precompile_headers(
            ccl-catch
            PUBLIC
            "pch.hpp"
            <boost/test/unit_test.hpp>
    )
endif ()

add_test(ccl-catch-test ccl-catch)
