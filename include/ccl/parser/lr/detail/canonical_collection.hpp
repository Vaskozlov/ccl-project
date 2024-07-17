#ifndef CCL_PROJECT_CANONICAL_COLLECTION_HPP
#define CCL_PROJECT_CANONICAL_COLLECTION_HPP

#include <ccl/parser/lr/detail/lr_item.hpp>

namespace ccl::parser
{
    struct CanonicalCollection
    {
        isl::Set<LrItem> items;
        Id id{};

        [[nodiscard]] auto operator==(const CanonicalCollection &other) const noexcept -> bool
        {
            return items == other.items;
        }

        [[nodiscard]] auto
            operator<=>(const CanonicalCollection &other) const noexcept -> std::weak_ordering
        {
            return items <=> other.items;
        }
    };

    template<typename T>
    struct CanonicalCollectionPrintWrapper
    {
        const CanonicalCollection &canonicalCollection;
    };
}// namespace ccl::parser

template<typename T>
class fmt::formatter<ccl::parser::CanonicalCollectionPrintWrapper<T>>
  : public fmt::formatter<std::string_view>
{
public:
    template<typename FmtContext>
    constexpr auto format(
        const ccl::parser::CanonicalCollectionPrintWrapper<T> &collection_print_wrapper,
        FmtContext &ctx) const
    {
        const auto &collection = collection_print_wrapper.canonicalCollection;

        return fmt::format_to(
            ctx.out(), "{}: {}", collection.id,
            std::views::transform(collection.items, [](const ccl::parser::LrItem &item) {
                return ccl::parser::LrItemPrintWrapper<T>(item);
            }));
    }
};

#endif /* CCL_PROJECT_CANONICAL_COLLECTION_HPP */
