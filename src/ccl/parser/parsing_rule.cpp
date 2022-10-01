#include <ccl/parser/parsing_rule.hpp>

namespace ccl::parser
{
    std::atomic<size_t> ParsingRule::uuid_counter{ 0 };// NOLINT (global variable)

    ParsingRule::ParsingRule(
        RuleId type_, std::string_view name_, UniquePtr<Node> (*rule_constructor_)(ParsingStack),
        InitializerList<RuleId> ids_to_constructs_,
        InitializerList<RuleId> ids_that_forbid_construction_)
      : ids_to_construct(ids_to_constructs_),
        ids_that_forbid_construction(ids_that_forbid_construction_), name(name_),
        rule_constructor(rule_constructor_), type(type_)
    {}

    auto ParsingRule::generateUuid() noexcept -> void
    {
        uuid = std::atomic_fetch_add_explicit(
            &ParsingRule::uuid_counter, 1UL, std::memory_order_relaxed);
    }
}// namespace ccl::parser
