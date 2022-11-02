cmake_minimum_required(VERSION 3.20.0)

project(ccl VERSION 0.0.2.0 LANGUAGES CXX)

include(GNUInstallDirs)

include(cmake/project-options.cmake)
include(cmake/compile-options.cmake)
include(cmake/libraries.cmake)

include(src/CMakeLists.txt)
include(catch/CMakeLists.txt)
include(bench/CMakeLists.txt)

install(TARGETS ccl
        EXPORT cclTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

install(FILES
        ${CCL_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

install(EXPORT cclTargets
        FILE cclTargets.cmake
        NAMESPACE ccl::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
        )

include(CMakePackageConfigHelpers)

set_property(TARGET ccl PROPERTY VERSION ${version})
set_property(TARGET ccl PROPERTY SOVERSION 3)

set_property(TARGET ccl PROPERTY
        INTERFACE_ccl_MAJOR_VERSION 3)

set_property(TARGET ccl APPEND PROPERTY
        COMPATIBLE_INTERFACE_STRING ccl_MAJOR_VERSION
        )

# generate the version file for the config file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/cclConfigVersion.cmake"
        VERSION "${version}"
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/cllConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ccl
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cclConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cclConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ccl
        )

# generate the export targets for the build tree
export(EXPORT cclTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/cclTargets.cmake"
        NAMESPACE ccl::
        )
