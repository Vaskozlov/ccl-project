#include <ccl/parser/node.hpp>

namespace ccl::parser
{
    auto Node::getPrintingPrefix(const std::string &prefix, bool is_left) -> std::string
    {
        return prefix + (is_left ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500");
    }

    auto Node::expandPrefix(const std::string &prefix, bool is_left, size_t extra_expansion)
        -> std::string
    {
        auto result = prefix + (is_left ? "\u2502   " : "    ");

        for (auto i = 0ZU; i != extra_expansion; ++i) {
            result.append("   ");
        }

        return result;
    }

    auto Node::print(const std::string &prefix, bool is_left) const -> void
    {
        fmt::print("{}{}\n", getPrintingPrefix(prefix, is_left), "Node");
    }

    auto TokenNode::print(const std::string &prefix, bool is_left) const -> void
    {
        fmt::print("{}{}\n", getPrintingPrefix(prefix, is_left), token.getRepr());
    }
}// namespace ccl::parser
