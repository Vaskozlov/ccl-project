option(USE_ADDRESS_SANITIZER "Use sanitizers" OFF)
option(USE_LIBCPP "Use libc++ when compiling with clang" OFF)
option(STRICT_COMPILATION "Enable -Werror strict compilation" OFF)

message(STATUS "CC " ${CMAKE_C_COMPILER})
message(STATUS "CXX " ${CMAKE_CXX_COMPILER})
message(STATUS "Use libcpp? " ${USE_LIBCPP})
message(STATUS "Use sanitizers? " ${USE_ADDRESS_SANITIZER})
message(STATUS "Strict compilation? " ${STRICT_COMPILATION})
