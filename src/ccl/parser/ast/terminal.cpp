#include <ccl/parser/ast/terminal.hpp>
#include <iostream>

namespace ccl::parser::ast
{
    auto Terminal::print(
        const std::string &prefix, const bool is_left,
        const std::function<std::string(SmallId)> &id_converter) const -> void
    {
        std::cout << fmt::format(
                         "{}-{}: {}", getPrintingPrefix(prefix, is_left), id_converter(getType()),
                         token.getRepr())
                  << std::endl;
    }
}// namespace ccl::parser::ast