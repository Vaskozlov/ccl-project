#include <ccl/parser/ast/non_terminal.hpp>
#include <iostream>

namespace ccl::parser::ast
{
    auto NonTerminal::joinWithNode(SharedNode<> node) -> void
    {
        if (auto *casted = dynamic_cast<NonTerminal *>(node.get()); casted != nullptr) {
            for (auto &elem : casted->nodes) {
                addNode(elem);
            }

            return;
        }

        addNode(std::move(node));
    }

    auto NonTerminal::print(
        const std::string &prefix, const bool is_left,
        const std::function<std::string(SmallId)> &id_converter) const -> void
    {
        const auto expanded_prefix = expandPrefix(prefix, is_left);
        std::cout << fmt::format(
                         "{}NodeSequence-{}", getPrintingPrefix(prefix, is_left),
                         id_converter(getType()))
                  << std::endl;


        for (const auto &node : nodes | std::views::take(nodes.size() - 1)) {
            node->print(expanded_prefix, true, id_converter);
        }

        if (!nodes.empty()) {
            nodes.back()->print(expanded_prefix, false, id_converter);
        }
    }
}// namespace ccl::parser::ast
