add_compile_definitions(BOOST_NO_CXX98_FUNCTION_BASE=false)

if (USE_LIBCPP)
    add_compile_options(-stdlib=libc++)
    add_link_options(-stdlib=libc++ -lc++abi)
else ()
    add_compile_options(-stdlib=libstdc++)
    add_link_options(-stdlib=libstdc++)
endif ()
