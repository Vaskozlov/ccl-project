#ifndef CCL_PROJECT_RULE_HPP
#define CCL_PROJECT_RULE_HPP

#include <ccl/parser/ast/allocator.hpp>
#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/ast/non_terminal.hpp>
#include <ccl/parser/types.hpp>
#include <functional>

namespace ccl::parser
{
    class Rule : public std::vector<Symbol>
    {
    private:
        std::vector<Symbol> firstSet;
        std::vector<Symbol> followSet;

    public:
        using vector::vector;

        explicit Rule(std::vector<Symbol> initial_value)
          : vector{std::move(initial_value)}
        {}

        [[nodiscard]] auto getFirstSet() const noexcept -> const auto &
        {
            return firstSet;
        }

        [[nodiscard]] auto getFollowSet() const noexcept -> const auto &
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
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_RULE_HPP */
