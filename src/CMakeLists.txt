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

file(
        GLOB_RECURSE
        CCL_HEADERS
        "include/ccl/*.hpp"
)

target_include_directories(ccl
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/include"
)

include(GNUInstallDirs)

install(TARGETS ccl
        EXPORT ccl
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES
        ${CCL_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ccl
        FILE ccl-config.cmake
        NAMESPACE ccl::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
)
