#ifndef CCL_PROJECT_SPPF_NODE_HPP
#define CCL_PROJECT_SPPF_NODE_HPP

#include <ccl/parser/ast/allocator.hpp>
#include <ccl/parser/rule_with_dot.hpp>

namespace ccl::parser
{
    struct SPPFNode
    {
        ast::SmallVectorOfNodes nodes;
        RuleWithDot rule{};
        Symbol production{};

        auto next(ast::SharedNode<> node) -> void
        {
            rule.dotPosition += 1;
            nodes.emplace_back(std::move(node));
        }

        [[nodiscard]] auto atDot() const -> Symbol
        {
            return rule.atDot();
        }

        [[nodiscard]] auto build() const -> ast::SharedNode<ast::NonTerminal>
        {
            return ast::SharedNode<ast::NonTerminal>{production, nodes};
        }

        [[nodiscard]] auto operator==(const SPPFNode &other) const -> bool
        {
            return rule.rule == other.rule.rule && nodes == other.nodes;
        }

        [[nodiscard]] auto operator<=>(const SPPFNode &other) const -> std::weak_ordering
        {
            if (const auto cmp = rule.rule <=> other.rule.rule; cmp != 0) {
                return cmp;
            }

            return nodes <=> other.nodes;
        }
    };
}// namespace ccl::parser

template<>
struct ankerl::unordered_dense::hash<ccl::parser::SPPFNode>
{
    using is_avalanching = void;

    [[nodiscard]] auto operator()(const ccl::parser::SPPFNode &sppf_node) const noexcept -> auto
    {
        return detail::wyhash::mix(
            reinterpret_cast<std::size_t>(sppf_node.rule.rule),
            detail::wyhash::hash(
                sppf_node.nodes.data(),
                sppf_node.nodes.size() * sizeof(ccl::parser::ast::SharedNode<>)));
    }
};


#endif /* CCL_PROJECT_SPPF_NODE_HPP */
