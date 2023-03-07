#include "ccl/utf8.hpp"

namespace ccl::utf8
{
    template auto appendUtf32ToUtf8Container<>(std::string &string, char32_t chr) -> void;
}// namespace ccl::utf8
