#ifndef CCL_PROJECT_REMOVE_ESCAPING_HPP
#define CCL_PROJECT_REMOVE_ESCAPING_HPP

#include <isl/string_view.hpp>

namespace ccl::text
{
    auto removeEscaping(
        isl::string_view input,
        std::span<isl::Pair<char32_t, char32_t>>
            additional_escaping_symbols) -> std::string;
}

#endif /* CCL_PROJECT_REMOVE_ESCAPING_HPP */
