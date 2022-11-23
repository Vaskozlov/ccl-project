#include <ccl/parser/parsing_rule.hpp>

namespace ccl::parser
{
    std::atomic<size_t> ParsingRule::uuid_counter{0};// NOLINT (global non-const variable)

    ParsingRule::ParsingRule(
        Id type_, std::string_view name_, UniquePtr<Node> (*constructor_)(ParsingStack),
        InitializerList<Id> ids_to_constructs_, InitializerList<Id> ids_that_forbid_construction_)
      : idsToConstruct{ids_to_constructs_}
      , idsThatForbidConstruction{ids_that_forbid_construction_}
      , name{name_}
      , constructor{constructor_}
      , type{type_}
    {}

    auto ParsingRule::generateUuid() noexcept -> void
    {
        uuid = std::atomic_fetch_add_explicit(
            &ParsingRule::uuid_counter, 1UL, std::memory_order_relaxed);
    }
}// namespace ccl::parser
