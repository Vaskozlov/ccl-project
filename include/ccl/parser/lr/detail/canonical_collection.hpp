#ifndef CCL_PROJECT_CANONICAL_COLLECTION_HPP
#define CCL_PROJECT_CANONICAL_COLLECTION_HPP

#include <ccl/parser/lr/lr_item.hpp>

namespace ccl::parser {
    struct CanonicalCollection {
        std::vector<LrItem> items;
        State id{};

        [[nodiscard]] auto operator==(const CanonicalCollection&other) const noexcept -> bool {
            return items == other.items;
        }
    };

    struct CanonicalCollectionPrintWrapper {
        const CanonicalCollection&canonicalCollection;
        std::function<std::string(SmallId)> idToStringConversionFunction;
    };
} // namespace ccl::parser

template<>
struct std::hash<ccl::parser::CanonicalCollection> {
    auto
    operator()(const ccl::parser::CanonicalCollection&collection) const noexcept -> std::size_t {
        return std::hash<std::vector<ccl::parser::LrItem>>{}(collection.items);
    }
};

template<>
struct fmt::formatter<ccl::parser::CanonicalCollectionPrintWrapper> : formatter<std::string_view> {
    static auto format(
        const ccl::parser::CanonicalCollectionPrintWrapper&collection_print_wrapper,
        format_context&ctx) -> format_context::iterator;
};

#endif /* CCL_PROJECT_CANONICAL_COLLECTION_HPP */
