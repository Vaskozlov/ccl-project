file(
        GLOB_RECURSE
        CCL_OBJECTS
        "src/ccl/*.cpp"
)

add_compile_definitions(BOOST_NO_CXX98_FUNCTION_BASE=false)

if (CREATE_STATIC_LIBRARY)
    add_library(ccl STATIC ${CCL_OBJECTS})
else ()
    add_library(ccl SHARED ${CCL_OBJECTS})
endif ()

target_link_libraries(
        ccl
        fmt::fmt-header-only
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
        fmt::fmt-header-only
)

target_link_libraries(
        fsc
        ccl
)

install(TARGETS ccl
        CONFIGURATIONS Debug
        RUNTIME DESTINATION Debug/bin)

install(TARGETS ccl
        CONFIGURATIONS Release
        RUNTIME DESTINATION Release/bin)
