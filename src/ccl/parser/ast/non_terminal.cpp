#include <ccl/parser/ast/non_terminal.hpp>
#include <iostream>

namespace ccl::parser::ast
{
    auto NonTerminal::joinWithNode(SharedNode<> node) -> void
    {
        if (auto *as_non_terminal = dynamic_cast<NonTerminal *>(node.get());
            as_non_terminal != nullptr) {
            for (const auto &elem : as_non_terminal->nodes) {
                addNode(elem);
            }

            return;
        }

        addNode(std::move(node));
    }

    auto NonTerminal::print(
        const std::string &prefix,
        const bool is_left,
        const std::function<std::string(SmallId)> &id_converter) const -> void
    {
        const auto expanded_prefix = expandPrefix(prefix, is_left);
        std::cout << fmt::format(
            "{}NodeSequence-{}", getPrintingPrefix(prefix, is_left), id_converter(getType()))
                  << std::endl;

        for (const auto &node : nodes | std::views::take(nodes.size() - 1)) {
            node->print(expanded_prefix, true, id_converter);
        }

        if (!nodes.empty()) {
            nodes.back()->print(expanded_prefix, false, id_converter);
        }
    }

    auto NonTerminal::getChildrenNodes() const -> isl::SmallFunction<SharedNode<>()>
    {
        return [index = 0U, &stored_nodes = nodes]() mutable -> SharedNode<> {
            if (index == stored_nodes.size()) {
                return nullptr;
            }

            return stored_nodes[index++];
        };
    }
} // namespace ccl::parser::ast
