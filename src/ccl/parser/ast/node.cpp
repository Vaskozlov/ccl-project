#include <ccl/parser/ast/node.hpp>

namespace ccl::parser::ast
{
    static auto operator*(std::string_view str, size_t repeat) -> std::string
    {
        auto result = std::string{};
        result.reserve(str.size() * repeat);

        for (auto i = isl::as<size_t>(0); i != repeat; ++i) {
            result.append(str);
        }

        return result;
    }

    auto Node::getPrintingPrefix(const std::string &prefix, bool is_left) -> std::string
    {
        return prefix + (is_left ? "├──" : "└──");
    }

    auto Node::expandPrefix(const std::string &prefix, bool is_left, size_t extra_expansion)
        -> std::string
    {
        static constexpr auto default_printing_shift = std::string_view{"   "};

        auto result = prefix + (is_left ? "│   " : "    ");
        result.append(default_printing_shift * extra_expansion);

        return result;
    }
}// namespace ccl::parser::ast