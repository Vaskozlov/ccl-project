#include <ccl/parser/node.hpp>

namespace ccl::parser
{
    using namespace std::string_literals;

    static auto operator*(const std::string &str, std::size_t repeat) -> std::string
    {
        auto ret = std::string{};
        ret.reserve(str.size() * repeat);

        for (size_t i = 0; i != repeat; ++i) {
            ret += str;
        }

        return ret;
    }

    auto Node::getPrintingPrefix(const std::string &prefix, bool is_left) -> std::string
    {
        return prefix + (is_left ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500");
    }

    auto Node::expandPrefix(const std::string &prefix, bool is_left, size_t extra_expansion)
        -> std::string
    {
        auto result = prefix + (is_left ? "\u2502   " : "    ");
        result.append("   "s * extra_expansion);

        return result;
    }

    auto Node::print(const std::string &prefix, bool is_left) const -> void
    {
        fmt::print("{}{}\n", getPrintingPrefix(prefix, is_left), "Node");
    }
}// namespace ccl::parser
