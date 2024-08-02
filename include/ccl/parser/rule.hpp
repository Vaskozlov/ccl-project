#ifndef CCL_PROJECT_RULE_HPP
#define CCL_PROJECT_RULE_HPP

#include "ccl/parser/ast/node.hpp"
#include "ccl/parser/ast/node_sequence.hpp"
#include "types.hpp"
#include <functional>

#define CCL_PARSER_RULE_CONSTRUCTOR_ARGUMENTS_DECL(PRODUCTION_NAME, NODES_ARGUMENT_NAME)           \
    <template<class> class SmartPointer>(                                                          \
        [[maybe_unused]] ccl::parser::Symbol PRODUCTION_NAME,                                      \
        [[maybe_unused]] std::vector<SmartPointer<ccl::parser::ast::Node>> NODES_ARGUMENT_NAME)


namespace ccl::parser
{
    template<typename... Ts>
    struct RuleConstructor : public Ts...
    {
        using Ts::operator()...;
    };

    template<typename... Ts>
    concept ConstructableWithUnique =
        requires(Symbol production, std::vector<ast::UnNodePtr> nodes) {
            std::declval<RuleConstructor<Ts...>>().operator()(production, std::move(nodes));
        };

    template<typename... Ts>
    concept ConstructableWithShared =
        requires(Symbol production, std::vector<ast::ShNodePtr> nodes) {
            std::declval<RuleConstructor<Ts...>>().operator()(production, std::move(nodes));
        };

    class Rule : public std::vector<Symbol>
    {
    private:
        std::function<ast::UnNodePtr(Symbol, std::vector<ast::UnNodePtr>)> uniqueConstructor;
        std::function<ast::ShNodePtr(Symbol, std::vector<ast::ShNodePtr>)> sharedConstructor;

        template<typename... Ts>
        explicit Rule(std::vector<Symbol> rule, RuleConstructor<Ts...> constructor)
          : std::vector<Symbol>{std::move(rule)}
        {
            if constexpr (ConstructableWithUnique<Ts...>) {
                uniqueConstructor =
                    [constructor](Symbol production, std::vector<ast::UnNodePtr> nodes) {
                        return constructor(production, std::move(nodes));
                    };
            }

            if constexpr (ConstructableWithShared<Ts...>) {
                sharedConstructor =
                    [constructor](Symbol production, std::vector<ast::ShNodePtr> nodes) {
                        return constructor(production, std::move(nodes));
                    };
            }
        }

    public:
        template<typename... Ts>
        explicit Rule(std::vector<Symbol> rule)
          : Rule{
                std::move(rule),
                RuleConstructor{
                    [] CCL_PARSER_RULE_CONSTRUCTOR_ARGUMENTS_DECL(production, nodes) {
                        return isl::makeUnique<ast::NodeSequence<SmartPointer>>(
                            production, std::move(nodes));
                    },
                },
            }
        {}

        template<typename... Ts>
        explicit Rule(std::vector<Symbol> rule, Ts &&...constructors)
          : Rule{std::move(rule), RuleConstructor{std::forward<Ts>(constructors)...}}
        {}

        Rule(const Rule &other) = default;
        Rule(Rule &&other) noexcept = default;

        auto operator=(const Rule &) -> Rule & = default;
        auto operator=(Rule &&) noexcept -> Rule & = default;

        template<template<class> class SmartPointer>
        [[nodiscard]] auto construct(Symbol production, std::vector<SmartPointer<ast::Node>> nodes)
            const -> SmartPointer<ast::Node>
        {
            if constexpr (std::same_as<SmartPointer<ast::Node>, ast::UnNodePtr>) {
                return uniqueConstructor(production, std::move(nodes));
            } else {
                return sharedConstructor(production, std::move(nodes));
            }
        }
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_RULE_HPP */
