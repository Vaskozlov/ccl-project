file(
        GLOB_RECURSE
        CCL_OBJECTS
        "src/ccl/*.cpp"
)

if (CREATE_STATIC_LIBRARY)
    add_library(ccl STATIC ${CCL_OBJECTS})
else ()
    add_library(ccl SHARED ${CCL_OBJECTS})
endif ()

target_link_libraries(
        ccl
        fmt::fmt
)

if (${PRECOMPILED_HEADERS})
    target_precompile_headers(
            ccl
            PUBLIC
            "pch.hpp"
    )
endif ()

include(programs/CMakeLists.txt)

file(
        GLOB_RECURSE
        FSC_OBJECTS
        "src/fsc/*.cpp"
)

if (CREATE_STATIC_LIBRARY)
    add_library(fsc STATIC ${FSC_OBJECTS})
else ()
    add_library(fsc SHARED ${FSC_OBJECTS})
endif ()

target_link_libraries(
        fsc
        fmt::fmt
)

target_link_libraries(
        fsc
        ccl
)

if (${PRECOMPILED_HEADER})
    target_precompile_headers(
            fsc
            PUBLIC
            "pch.hpp"
            "include/ccl/lex/lexical_analyzer.hpp"
    )
endif ()
