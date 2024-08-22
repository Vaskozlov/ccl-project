#ifndef CCL_PROJECT_SPPF_NODE_HPP
#define CCL_PROJECT_SPPF_NODE_HPP

#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/rule_with_dot.hpp>

namespace ccl::parser
{
    struct SPPFNode
    {
        RuleWithDot rule{};
        Symbol production{};
        std::vector<ast::Node *> nodes;

        auto next(ast::Node* node) -> void {
            rule.dotPosition += 1;
            nodes.emplace_back(node);
        }

        [[nodiscard]] auto atDot() const -> Symbol {
            return rule.atDot();
        }

        [[nodiscard]] auto build() const -> ast::Node* {
            auto* new_node = rule.rule->construct(production, nodes);
            nodes.front()->getLifetimeManager()->insert(new_node);
            return new_node;
        }

        [[nodiscard]] auto operator<=>(const SPPFNode&other) const -> std::weak_ordering = default;
    };
} // namespace ccl::parser

#endif /* CCL_PROJECT_SPPF_NODE_HPP */
