#ifndef CCL_PROJECT_SPPF_NODE_HPP
#define CCL_PROJECT_SPPF_NODE_HPP

#include <ccl/parser/ast/allocator.hpp>
#include <ccl/parser/rule_with_dot.hpp>

namespace ccl::parser
{
    struct SPPFNode
    {
        std::vector<ast::SharedNode<>> nodes;
        RuleWithDot rule{};
        Symbol production{};

        auto next(ast::SharedNode<> node) -> void {
            rule.dotPosition += 1;
            nodes.emplace_back(node);
        }

        [[nodiscard]] auto atDot() const -> Symbol {
            return rule.atDot();
        }

        [[nodiscard]] auto build() const -> ast::SharedNode<> {
            return rule.rule->construct(production, nodes);
        }

        [[nodiscard]] auto operator<=>(const SPPFNode&other) const -> std::weak_ordering = default;
    };
} // namespace ccl::parser

#endif /* CCL_PROJECT_SPPF_NODE_HPP */
