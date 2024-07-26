#include <ccl/parser/ast/node_of_nodes.hpp>
#include <iostream>

namespace ccl::parser::ast
{
    auto NodeOfNodes::print(
        const std::string &prefix, bool is_left,
        std::function<std::string(Id)> id_converter) const -> void
    {
        const auto expanded_prefix = expandPrefix(prefix, is_left);
        std::cout << fmt::format(
                         "{}NodeSequence-{}", getPrintingPrefix(prefix, is_left),
                         id_converter(getType()))
                  << std::endl;

        const auto &nodes = getNodesPointers();

        for (const auto *node : nodes | std::ranges::views::take(nodes.size() - 1)) {
            node->print(expanded_prefix, true, id_converter);
        }

        if (!nodes.empty()) {
            const auto *node = nodes.back();
            node->print(expanded_prefix, false, id_converter);
        }
    }
}// namespace ccl::parser::ast