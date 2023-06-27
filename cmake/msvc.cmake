add_compile_options(/W4 /Zc:__cplusplus /utf-8)

if (CCL_ADDRESS_SANITIZER)
    add_compile_options(/fsanitize=address)
endif ()

