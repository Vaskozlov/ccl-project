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

isl_set_target_flags(
        ccl-catch
        ${CCL_STRICT_COMPILATION}
        ""
        ""
        ""
        ""
        ${CCL_HARDENING}
        ${CCL_LIBCPP}
        ${CCL_UNITY_BUILD}
)

target_link_libraries(
        ccl-catch
        ccl
        astlang2
        fmt::fmt-header-only
        Catch2::Catch2WithMain
        astlang2
)

if (CCL_PRECOMPILED_HEADERS)
    target_precompile_headers(
            ccl-catch
            REUSE_FROM
            ccl
    )
endif ()

add_test(ccl-catch-test ccl-catch)