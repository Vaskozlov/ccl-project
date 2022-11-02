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

set(include_dest "include/ccl")
set(ccl_lib_dest "lib/ccl/${CMAKE_BUILD_TYPE}")

file(
        GLOB_RECURSE
        CCL_HEADERS
        "include/ccl/*.hpp"
)

install(TARGETS ccl EXPORT ccl DESTINATION "${ccl_lib_dest}")
install(FILES ${CCL_HEADERS} DESTINATION "${include_dest}")
