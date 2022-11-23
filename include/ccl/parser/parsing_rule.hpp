#ifndef CCL_PROJECT_PARSING_RULE_HPP
#define CCL_PROJECT_PARSING_RULE_HPP

#include <atomic>
#include <ccl/parser/types.hpp>

namespace ccl::parser
{
    struct Parser;
    class ParsingRules;

    struct ParsingStack
    {
        explicit ParsingStack(Stack &stack_)
          : stack{stack_}
        {}

        auto pop() -> UniquePtr<Node>
        {
            auto node = std::move(stack.back());
            stack.pop_back();
            return node;
        }

    private:
        Stack &stack;
    };

    class ParsingRule
    {
        friend Parser;
        friend ParsingRules;

        static std::atomic<size_t> uuid_counter;// NOLINT

        SmallVector<Id> idsToConstruct{};
        FlatSet<Id> idsThatForbidConstruction{};
        string_view name{"set name for ccl::parser::ParsingRule"};
        UniquePtr<Node> (*constructor)(ParsingStack) = nullptr;
        Id type{};
        size_t uuid{};

    public:
        ParsingRule() = default;

        ParsingRule(
            Id type_, std::string_view name_, UniquePtr<Node> (*constructor_)(ParsingStack),
            InitializerList<Id> ids_to_constructs_,
            InitializerList<Id> ids_that_forbid_construction_ = {});

        [[nodiscard]] auto getName() const noexcept -> string_view
        {
            return name;
        }

        [[nodiscard]] auto getType() const noexcept -> Id
        {
            return type;
        }

        [[nodiscard]] auto getIdsToConstruct() const noexcept -> const SmallVector<Id> &
        {
            return idsToConstruct;
        }

        [[nodiscard]] auto getIdsThatForbidConstruction() const noexcept -> const FlatSet<Id> &
        {
            return idsThatForbidConstruction;
        }

    private:
        auto generateUuid() noexcept -> void;

        [[nodiscard]] auto canNotBeConstructed(Id future_id) const noexcept -> bool
        {
            return idsThatForbidConstruction.contains(future_id);
        }
    };

}// namespace ccl::parser

template<>
struct fmt::formatter<ccl::parser::ParsingRule> : fmt::formatter<std::string_view>
{
    auto format(const ccl::parser::ParsingRule &rule, format_context &ctx) const
    {
        return formatter<std::string_view>::format(as<std::string_view>(rule.getName()), ctx);
    }
};

#endif /* CCL_PROJECT_PARSING_RULE_HPP */
