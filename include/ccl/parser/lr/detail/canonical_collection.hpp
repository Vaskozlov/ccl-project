#ifndef CCL_PROJECT_CANONICAL_COLLECTION_HPP
#define CCL_PROJECT_CANONICAL_COLLECTION_HPP

#include <ccl/parser/grammar_slot.hpp>
#include <list>

namespace ccl::parser
{
    struct CanonicalCollection
    {
        std::vector<GrammarSlot> items;
        State id{};

        [[nodiscard]] auto operator==(const CanonicalCollection &other) const noexcept -> bool
        {
            return items == other.items;
        }
    };

    struct CanonicalCollectionPrintWrapper
    {
        const CanonicalCollection &canonicalCollection;
        std::function<std::string(SmallId)> idToStringConversionFunction;
    };
}// namespace ccl::parser

template<>
struct std::hash<ccl::parser::CanonicalCollection>
{
    auto operator()(const ccl::parser::CanonicalCollection &collection) const -> std::size_t
    {
        return std::hash<std::vector<ccl::parser::GrammarSlot>>{}(collection.items);
    }
};

template<>
class fmt::formatter<ccl::parser::CanonicalCollectionPrintWrapper>
  : public fmt::formatter<std::string_view>
{
public:
    template<typename FmtContext>
    constexpr auto format(
        const ccl::parser::CanonicalCollectionPrintWrapper &collection_print_wrapper,
        FmtContext &ctx) const
    {
        const auto &collection = collection_print_wrapper.canonicalCollection;

        return fmt::format_to(
            ctx.out(), "{}: {}", collection.id,
            std::views::transform(
                collection.items,
                [&collection_print_wrapper](const ccl::parser::GrammarSlot &item) {
                    return ccl::parser::GrammarSlotPrintWrapper(
                        item, collection_print_wrapper.idToStringConversionFunction);
                }));
    }
};

#endif /* CCL_PROJECT_CANONICAL_COLLECTION_HPP */
