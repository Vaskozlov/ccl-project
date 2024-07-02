#include <ccl/parser/ast/node_sequence.hpp>
#include <iostream>
#include <ranges>

namespace ccl::parser::ast
{
    auto NodeSequence::print(
        const std::string &prefix, bool is_left,
        std::function<isl::string_view(Id)> id_converter) const -> void
    {
        const auto expanded_prefix = expandPrefix(prefix, is_left);
        std::cout << fmt::format(
                         "{}NodeSequence-{}", getPrintingPrefix(prefix, is_left),
                         id_converter(getId()))
                  << std::endl;

        for (const auto &node : nodes | std::ranges::views::take(nodes.size() - 1)) {
            node->print(expanded_prefix, true, id_converter);
        }

        if (!nodes.empty()) {
            const auto &node = nodes.back();
            node->print(expanded_prefix, false, id_converter);
        }
    }

}// namespace ccl::parser::ast