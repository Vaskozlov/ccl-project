#ifndef CCL_PROJECT_RULE_HPP
#define CCL_PROJECT_RULE_HPP

#include <ccl/parser/ast/allocator.hpp>
#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/ast/node_of_nodes.hpp>
#include <ccl/parser/types.hpp>
#include <functional>

namespace ccl::parser
{
    class Rule : public std::vector<Symbol>
    {
    public:
        using RuleBuilderFunction = std::function<ast::SharedNode<ast::NodeOfNodes>(
            Symbol, std::vector<ast::SharedNode<ast::Node>>)>;

    private:
        std::vector<Symbol> firstSet;
        std::vector<Symbol> followSet;
        RuleBuilderFunction builder;

    public:
        explicit Rule(std::vector<Symbol> rule, RuleBuilderFunction rule_builder = defaultBuilder)
          : std::vector<Symbol>{std::move(rule)}
          , builder{std::move(rule_builder)}
        {}

        [[nodiscard]] auto construct(Symbol production, std::vector<ast::SharedNode<>> nodes) const
            -> ast::SharedNode<ast::NodeOfNodes>
        {
            return builder(production, std::move(nodes));
        }

        [[nodiscard]] auto getFirstSet() const noexcept -> const std::vector<Symbol> &
        {
            return firstSet;
        }

        [[nodiscard]] auto getFollowSet() const noexcept -> const std::vector<Symbol> &
        {
            return followSet;
        }

        auto setFirstSet(isl::RangeOf<Symbol> auto &&first_set_addition) -> void
        {
            firstSet.clear();

            for (Symbol symbol : first_set_addition) {
                firstSet.emplace_back(symbol);
            }
        }

        auto setFollowSet(isl::RangeOf<Symbol> auto &&follow_set_addition) -> void
        {
            followSet.clear();

            for (Symbol symbol : follow_set_addition) {
                followSet.emplace_back(symbol);
            }
        }

    private:
        static auto defaultBuilder(Symbol production, std::vector<ast::SharedNode<ast::Node>> nodes)
            -> ast::SharedNode<ast::NodeOfNodes>
        {
            return ast::SharedNode<ast::NodeOfNodes>{production, std::move(nodes)};
        }
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_RULE_HPP */
