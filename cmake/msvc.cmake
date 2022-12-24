add_compile_options(/W4 /Zc:__cplusplus /utf-8)

ccl_check_cxx_flag_support(W4 TRUE)
ccl_check_cxx_flag_support(utf-8 TRUE)
ccl_check_cxx_flag_support(Zc:__cplusplus TRUE)
ccl_check_cxx_flag_support(WX ${STRICT_COMPILATION})

if (USE_ADDRESS_SANITIZER)
    add_compile_options(/fsanitize=address)
endif ()
