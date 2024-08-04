cmake_minimum_required(VERSION 3.20.0)

project(ccl-project VERSION 0.0.2.1 LANGUAGES CXX)

include(GNUInstallDirs)
include(FetchContent)
include(CMakePackageConfigHelpers)

include(cmake/Libraries.cmake)
include(cmake/ProjectOptions.cmake)

include(programs/CMakeLists.txt)
include(src/CMakeLists.txt)
include(catch/CMakeLists.txt)

install(TARGETS ccl ccl-lexer unordered_dense
        EXPORT cclTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ccl
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")

install(EXPORT cclTargets
        FILE cclTargets.cmake
        NAMESPACE ccl::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ccl
)

set_property(TARGET ccl PROPERTY VERSION ${version})

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/cclConfigVersion.cmake"
        VERSION "${version}"
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/cclConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ccl
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/cclConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cclConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ccl
)

export(EXPORT cclTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/cclTargets.cmake"
        NAMESPACE ccl::
)
