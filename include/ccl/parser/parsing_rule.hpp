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
        explicit ParsingStack(Stack &stack_) : stack(stack_)
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

        SmallVector<RuleId, 4> ids_to_construct{};
        FlatSet<RuleId> ids_that_forbid_construction{};
        string_view name{ "set name for ccl::parser::ParsingRule" };
        UniquePtr<Node> (*rule_construction_call)(ParsingStack) = nullptr;
        RuleId type{};
        size_t uuid{};

    public:
        ParsingRule() = default;

        ParsingRule(
            RuleId type_, std::string_view name_,
            UniquePtr<Node> (*rule_construction_call_)(ParsingStack),
            InitializerList<RuleId> ids_to_constructs_,
            InitializerList<RuleId> ids_that_forbid_construction_ = {});

        [[nodiscard]] auto getName() const noexcept -> string_view
        {
            return name;
        }

        [[nodiscard]] auto getType() const noexcept -> RuleId
        {
            return type;
        }

        [[nodiscard]] auto getIdsToConstruct() const noexcept -> const SmallVector<RuleId, 4> &
        {
            return ids_to_construct;
        }

        [[nodiscard]] auto getIdsThatForbidConstruction() const noexcept -> const FlatSet<RuleId> &
        {
            return ids_that_forbid_construction;
        }

    private:
        auto generateUuid() noexcept -> void;

        [[nodiscard]] auto canNotBeConstructed(RuleId future_id) const noexcept -> bool
        {
            return ids_that_forbid_construction.contains(future_id);
        }
    };

}// namespace ccl::parser

template<>
struct fmt::formatter<ccl::parser::ParsingRule> : fmt::formatter<std::string_view>
{
    auto format(const ccl::parser::ParsingRule &rule, format_context &ctx) const
    {
        return formatter<std::string_view>::format(
            static_cast<std::string_view>(rule.getName()), ctx);
    }
};

#endif /* CCL_PROJECT_PARSING_RULE_HPP */
