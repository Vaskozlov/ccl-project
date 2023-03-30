include(CheckCXXCompilerFlag)

ccl_check_cxx_flag_support(Wall TRUE)
ccl_check_cxx_flag_support(Wextra TRUE)
ccl_check_cxx_flag_support(Wshadow ${CCL_WSHADOW})
ccl_check_cxx_flag_support(Wuseless-cast TRUE)
ccl_check_cxx_flag_support(Werror ${CCL_STRICT_COMPILATION})

ccl_check_cxx_flag_support(pedantic TRUE)
ccl_check_cxx_flag_support(wpedantic TRUE)

ccl_check_cxx_flag_support(Wmove TRUE)
ccl_check_cxx_flag_support(Wlogical-op TRUE)
ccl_check_cxx_flag_support(Wuninitialized TRUE)
ccl_check_cxx_flag_support(Wold-style-cast TRUE)
ccl_check_cxx_flag_support(Wnull-dereference TRUE)

ccl_check_cxx_flag_support(Wduplicated-cond TRUE)
ccl_check_cxx_flag_support(Wduplicated-branches TRUE)
ccl_check_cxx_flag_support(Wmisleading-indentation TRUE)

ccl_check_cxx_flag_support(Wconversion TRUE)
ccl_check_cxx_flag_support(Wcast-align TRUE)
ccl_check_cxx_flag_support(Wcast-align=strict TRUE)
ccl_check_cxx_flag_support(Wfloat-equal TRUE)
ccl_check_cxx_flag_support(Wsign-conversion TRUE)
ccl_check_cxx_flag_support(Wdouble-promotion TRUE)
ccl_check_cxx_flag_support(Wfloat-conversion TRUE)

ccl_check_cxx_flag_support(Wformat=2 TRUE)

ccl_check_cxx_flag_support(Wthread-safety TRUE)

ccl_check_cxx_flag_support(Wnon-virtual-dtor TRUE)
ccl_check_cxx_flag_support(Woverloaded-virtual TRUE)

if (CCL_ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address,undefined,leak,alignment,builtin)
    add_link_options(-fsanitize=address,undefined,leak,alignment,builtin)
elseif (CCL_THREAD_SANITIZER)
    add_compile_options(-fsanitize=thread,alignment,address,builtin)
    add_link_options(-fsanitize=thread,alignment,address,builtin)
else ()
    ccl_check_cxx_flag_support(fsanitize-minimal-runtime TRUE)
endif ()
