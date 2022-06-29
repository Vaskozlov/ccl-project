set(CMAKE_CXX_STANDARD 23)

if (MSVC)
    include(cmake/msvc.cmake)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    include(cmake/unix.cmake)
    include(cmake/clang.cmake)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    include(cmake/unix.cmake)
    include(cmake/gcc.cmake)
endif ()

include_directories(include)
