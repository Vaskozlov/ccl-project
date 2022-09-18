#include <ccl/parser/node.hpp>

namespace ccl::parser
{
    auto Node::print(const std::string &prefix, bool is_left) const -> void
    {
        fmt::print("{}{}\n", getPrintingPrefix(prefix, is_left), "Node");
    }

    auto TokenNode::print(const std::string &prefix, bool is_left) const -> void
    {
        fmt::print("{}{}\n", getPrintingPrefix(prefix, is_left), token.getRepr());
    }
}// namespace ccl::parser
