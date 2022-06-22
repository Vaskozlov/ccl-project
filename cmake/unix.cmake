add_compile_options(
        -std=c++2b -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -pedantic -Wold-style-cast
        -Wcast-align -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wdouble-promotion
        -Wformat=2 -Wuninitialized -Wfloat-conversion
)

if (CERBERUS_STRICT_COMPILATION)
    add_compile_options(-Werror)
endif ()

if (USE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined,leak)
    add_link_options(-fsanitize=address,undefined,leak)
endif ()