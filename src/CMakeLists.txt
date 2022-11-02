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

set(tool_dest "bin")
set(include_dest "include/ccl")

file(
        GLOB_RECURSE
        CCL_HEADERS
        "include/ccl/*.hpp"
)

install(TARGETS ccl DESTINATION bin)
install(FILES ${CCL_HEADERS} DESTINATION "${include_dest}")
