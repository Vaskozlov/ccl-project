#ifndef CCL_PROJECT_RULE_HPP
#define CCL_PROJECT_RULE_HPP

#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/ast/node_of_nodes.hpp>
#include <ccl/parser/types.hpp>
#include <functional>

#define CCL_PARSER_RULE_CONSTRUCTOR_ARGUMENTS_DECL(PRODUCTION_NAME, NODES_ARGUMENT_NAME)           \
    <template<class> class SmartPointer>(                                                          \
        [[maybe_unused]] ccl::parser::Symbol PRODUCTION_NAME,                                      \
        [[maybe_unused]] std::vector<ccl::parser::ast::Node *>                                     \
            NODES_ARGUMENT_NAME)


namespace ccl::parser
{
    class Rule : public std::vector<Symbol>
    {
    public:
        using RuleBuilderFunction = std::function<ast::Node *(Symbol, std::vector<ast::Node *>)>;

    private:
        RuleBuilderFunction builder;

    public:
        explicit Rule(
            std::vector<Symbol>
                rule,
            RuleBuilderFunction rule_builder =
                [](Symbol production, std::vector<ast::Node *> nodes) {
                    return new ast::NodeOfNodes{production, std::move(nodes)};
                })
          : std::vector<Symbol>{std::move(rule)}
          , builder{std::move(rule_builder)}
        {}

        Rule(const Rule &other) = default;
        Rule(Rule &&other) noexcept = default;

        auto operator=(const Rule &) -> Rule & = default;
        auto operator=(Rule &&) noexcept -> Rule & = default;

        [[nodiscard]] auto
            construct(Symbol production, std::vector<ast::Node *> nodes) const -> ast::Node *
        {
            return builder(production, std::move(nodes));
        }
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_RULE_HPP */
