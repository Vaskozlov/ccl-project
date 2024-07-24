#ifndef CCL_PROJECT_CANONICAL_COLLECTION_HPP
#define CCL_PROJECT_CANONICAL_COLLECTION_HPP

#include <ccl/parser/lr/detail/lr_item.hpp>

namespace ccl::parser
{
    struct CanonicalCollection
    {
        isl::UnorderedSet<LrItem> items;
        Id id{};

        [[nodiscard]] auto operator==(const CanonicalCollection &other) const noexcept -> bool
        {
            return items == other.items;
        }
    };

    struct CanonicalCollectionPrintWrapper
    {
        const CanonicalCollection &canonicalCollection;
        std::function<std::string(Id)> idToStringConversionFunction;
    };
}// namespace ccl::parser

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
                collection.items, [&collection_print_wrapper](const ccl::parser::LrItem &item) {
                    return ccl::parser::LrItemPrintWrapper(
                        item, collection_print_wrapper.idToStringConversionFunction);
                }));
    }
};

#endif /* CCL_PROJECT_CANONICAL_COLLECTION_HPP */
