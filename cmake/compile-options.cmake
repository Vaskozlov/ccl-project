function(ccl_check_cxx_flag_support flag enable_condition)
    set(VARIABLE_NAME "HAVE_${flag}")

    if (MSVC)
        set(COMPILER_OPTION "/${flag}")
    else ()
        set(COMPILER_OPTION "-${flag}")
    endif ()

    string(REPLACE "-" "_" VARIABLE_NAME ${VARIABLE_NAME})
    string(REPLACE ":" "_COLON_" VARIABLE_NAME ${VARIABLE_NAME})
    string(TOUPPER ${VARIABLE_NAME} VARIABLE_NAME)

    if (MSVC)
        set(${VARIABLE_NAME} TRUE)
    else ()
        check_cxx_compiler_flag(${COMPILER_OPTION} ${VARIABLE_NAME})
    endif ()

    if (${enable_condition} AND ${VARIABLE_NAME})
        add_compile_options(${COMPILER_OPTION})
    endif ()

endfunction()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(CCL_DEBUG=1)
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    include(cmake/msvc.cmake)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    include(cmake/unix.cmake)
    include(cmake/clang.cmake)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    include(cmake/unix.cmake)
endif ()
