#ifndef CCL_PROJECT_RULE_HPP
#define CCL_PROJECT_RULE_HPP

#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/types.hpp>
#include <functional>

#define CCL_PARSER_RULE_CONSTRUCTOR_ARGUMENTS_DECL(PRODUCTION_NAME, NODES_ARGUMENT_NAME)           \
    <template<class> class SmartPointer>(                                                          \
        [[maybe_unused]] ccl::parser::Symbol PRODUCTION_NAME,                                      \
        [[maybe_unused]] isl::Vector<SmartPointer<ccl::parser::ast::Node>> NODES_ARGUMENT_NAME)


namespace ccl::parser
{
    template<typename... Ts>
    struct RuleConstructor : public Ts...
    {
        using Ts::operator()...;
    };

    template<typename... Ts>
    concept ConstructableWithUnique =
        requires(Symbol production, isl::Vector<ast::UnNodePtr> nodes) {
            std::declval<RuleConstructor<Ts...>>().operator()(production, std::move(nodes));
        };

    template<typename... Ts>
    concept ConstructableWithShared =
        requires(Symbol production, isl::Vector<ast::ShNodePtr> nodes) {
            std::declval<RuleConstructor<Ts...>>().operator()(production, std::move(nodes));
        };

    class Rule : public isl::Vector<Symbol>
    {
    private:
        std::function<ast::UnNodePtr(Symbol, isl::Vector<ast::UnNodePtr>)> uniqueConstructor;
        std::function<ast::ShNodePtr(Symbol, isl::Vector<ast::ShNodePtr>)> sharedConstructor;
        std::size_t ruleHash{};

    private:
        template<typename... Ts>
        explicit Rule(isl::Vector<Symbol> rule, RuleConstructor<Ts...> constructor)
          : isl::Vector<Symbol>{std::move(rule)}
        {
            if constexpr (ConstructableWithUnique<Ts...>) {
                uniqueConstructor =
                    [constructor](Symbol production, isl::Vector<ast::UnNodePtr> nodes) {
                        return constructor(production, std::move(nodes));
                    };
            }

            if constexpr (ConstructableWithShared<Ts...>) {
                sharedConstructor =
                    [constructor](Symbol production, isl::Vector<ast::ShNodePtr> nodes) {
                        return constructor(production, std::move(nodes));
                    };
            }

            for (auto symbol : rule) {
                ruleHash *= 31U;
                ruleHash += symbol;
            }
        }

    public:
        template<typename... Ts>
        explicit Rule(isl::Vector<Symbol> rule)
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
        explicit Rule(isl::Vector<Symbol> rule, Ts &&...constructors)
          : Rule{std::move(rule), RuleConstructor{std::forward<Ts>(constructors)...}}
        {}

        [[nodiscard]] auto getHash() const noexcept -> std::size_t
        {
            return ruleHash;
        }

        template<template<class> class SmartPointer>
        [[nodiscard]] auto construct(Symbol production, isl::Vector<SmartPointer<ast::Node>> nodes)
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

template<>
struct std::hash<ccl::parser::Rule>
{
    auto operator()(const ccl::parser::Rule &rule) const noexcept -> std::size_t
    {
        return rule.getHash();
    }
};

#endif /* CCL_PROJECT_RULE_HPP */
