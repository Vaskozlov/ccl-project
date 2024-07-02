#ifndef CCL_PROJECT_SCANNER_TEMPLATE_HPP
#define CCL_PROJECT_SCANNER_TEMPLATE_HPP

#include <ccl/lexer/dot_item/item_concept.hpp>

namespace ccl::lexer::dot_item
{
    /**
     * Functions to override:
     *  - scanIteration(DotItemConcept&)
     *  - onIteration(ResultOfScanIteration &)
     *  - constructResult(std::size_t totally_bytes_skipped)
     * @tparam CRTP
     * @tparam RT
     */
    template<typename CRTP, typename RT>
        requires isl::DerivedOrSame<RT, ScanResult>
    class CrtpScanner
    {
    public:
        using ForkedGenerator = typename text::TextIterator::ForkedTextIterator;

    protected:
        Closure closure;
        // NOLINTNEXTLINE reference
        const DotItemConcept &item;
        // NOLINTNEXTLINE reference
        ForkedGenerator &textIterator;

    public:
        CrtpScanner(
            Closure item_closure, const DotItemConcept &item_concept,
            ForkedGenerator &text_iterator)
          : closure{item_closure}
          , item{item_concept}
          , textIterator{text_iterator}
        {}

        [[nodiscard]] auto scan() -> RT
        {
            auto times = isl::as<std::size_t>(0);
            auto totally_skipped = isl::as<std::size_t>(0);

            while (!textIterator.isEOI() && times < closure.to) {
                auto &&scan_result = toParent().scanIteration(item);

                if (scan_result.isFailure()) {
                    break;
                }

                toParent().onIteration(scan_result);

                textIterator.skip(scan_result.getBytesCount());
                totally_skipped += scan_result.getBytesCount();
                ++times;
            }

            if (closure.isInClosure(times)) {
                return toParent().constructResult(totally_skipped);
            }

            return RT::failure();
        }

    private:
        CCL_DECL auto toParent() noexcept CCL_LIFETIMEBOUND -> CRTP &
        {
            return static_cast<CRTP &>(*this);
        }

        CCL_DECL auto toParent() const noexcept CCL_LIFETIMEBOUND -> const CRTP &
        {
            return static_cast<const CRTP &>(*this);
        }
    };
}// namespace ccl::lexer::dot_item

#endif /* CCL_PROJECT_SCANNER_TEMPLATE_HPP */
