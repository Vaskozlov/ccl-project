#ifndef CCL_PROJECT_SCANNER_TEMPLATE_HPP
#define CCL_PROJECT_SCANNER_TEMPLATE_HPP

#include <ccl/lexer/rule/rule_block_interface.hpp>

namespace ccl::lexer::rule
{
    /**
     * Functions to override:
     *  - scanIteration(RuleBlockInterface&) const
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
        const RuleBlockInterface &item;
        // NOLINTNEXTLINE reference
        ForkedGenerator &textIterator;

    public:
        CrtpScanner(
            Closure item_closure, const RuleBlockInterface &item_concept,
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
                auto &&scan_result = callScanIteration();

                if (scan_result.isFailure()) {
                    break;
                }

                toParent().onIteration(scan_result);
                callOnIteration(scan_result);

                textIterator.skip(scan_result.getBytesCount());
                totally_skipped += scan_result.getBytesCount();
                ++times;
            }

            if (closure.isInClosure(times)) {
                return callConstructResult(totally_skipped);
            }

            return RT::failure();
        }

    private:
        [[nodiscard]] auto callScanIteration() const -> RT
        {
            return toParent().scanIteration(item);
        }

        auto callOnIteration(RT &scan_result) -> void
        {
            return toParent().onIteration(scan_result);
        }

        [[nodiscard]] auto callConstructResult(std::size_t totally_skipped) -> RT
        {
            return toParent().constructResult(totally_skipped);
        }

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
