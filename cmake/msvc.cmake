add_compile_options(/W4 /Zc:__cplusplus)

if (STRICT_COMPILATION)
    add_compile_options(/WX)
endif ()

if (USE_ADDRESS_SANITIZER)
    add_compile_options(/fsanitize=address)
endif ()

include_directories(${Boost_INCLUDE_DIR})
