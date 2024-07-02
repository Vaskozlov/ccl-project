#include <ccl/parser/ast/value.hpp>
#include <iostream>

namespace ccl::parser::ast
{
    auto ValueNode::print(
        const std::string &prefix, bool is_left,
        std::function<isl::string_view(Id)> id_converter) const -> void
    {
        std::cout << fmt::format(
                         "{}-{}: {}", getPrintingPrefix(prefix, is_left), id_converter(getId()),
                         repr)
                  << std::endl;
    }
}// namespace ccl::parser::ast