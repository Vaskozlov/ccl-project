export module ccl.lexer.dot_item.core:item_concept;

export import ccl.text;
export import :repetition;

export namespace ccl::lexer::dot_item
{
    enum struct ScanType : u16
    {
        MAIN,
        FORKED,
        SPECIAL
    };

    class DotItemConcept
    {
    public:
        using TextIterator = text::TextIterator;
        using ForkedGenerator = typename text::TextIterator::ForkedTextIterator;

        struct Flags
        {
            bool reversed = false;
            bool isPrefix = false;
            bool isPostfix = false;
            bool sequenceIsMultiline = false;
            bool noEscapingSymbols = false;
        };

    private:
        Repetition repetition{Repetition::basic()};
        Id id{};
        Flags flags;

    public:
        explicit DotItemConcept(Id item_id)
          : id{item_id}
        {}

        DotItemConcept(Id item_id, Flags item_flags)
          : id{item_id}
          , flags{item_flags}
        {}

        DotItemConcept(const DotItemConcept &) = default;
        DotItemConcept(DotItemConcept &&) noexcept = default;

        virtual ~DotItemConcept() = default;

        auto operator=(const DotItemConcept &) -> void = delete;
        auto operator=(DotItemConcept &&) noexcept -> void = delete;

        [[nodiscard]] auto getRepetition() const noexcept -> Repetition
        {
            return repetition;
        }

        [[nodiscard]] auto getFlags() const noexcept -> Flags
        {
            return flags;
        }

        [[nodiscard]] auto isReversed() const noexcept -> bool
        {
            return flags.reversed;
        }

        [[nodiscard]] auto hasPrefix() const noexcept -> bool
        {
            return flags.isPrefix;
        }

        [[nodiscard]] auto hasPostfix() const noexcept -> bool
        {
            return flags.isPostfix;
        }

        void setPrefix() noexcept
        {
            flags.isPrefix = true;
        }

        void setPostfix() noexcept
        {
            flags.isPostfix = true;
        }

        auto reverse() noexcept -> void
        {
            flags.reversed = !flags.reversed;
        }

        auto setRepetition(Repetition new_repetition) noexcept -> void
        {
            repetition = new_repetition;
        }

        [[nodiscard]] auto canBeOptimized() const noexcept -> bool
        {
            return (!isReversed() && (0 == repetition.from)) && empty();
        }

        [[nodiscard]] auto getId() const noexcept -> Id
        {
            return id;
        }

        static auto alwaysRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void
        {
            if (condition) [[unlikely]] {
                text_iterator.throwWarning(
                    text::AnalysisStage::LEXICAL_ANALYSIS, "item will be always recognized");
            }
        }

        static auto neverRecognizedSuggestion(TextIterator &text_iterator, bool condition) -> void
        {
            if (condition) [[unlikely]] {
                text_iterator.throwWarning(
                    text::AnalysisStage::LEXICAL_ANALYSIS, "item will never be recognized");
            }
        }

        [[nodiscard]] virtual auto empty() const noexcept -> bool = 0;

        [[nodiscard]] auto scan(ForkedGenerator text_iterator) const -> std::optional<std::size_t>
        {
            auto times = 0ZU;
            auto totally_skipped = 0ZU;

            while (!text_iterator.isEOI() && times < repetition.to) {
                auto chars_to_skip = scanIteration(text_iterator);

                if (!chars_to_skip.has_value()) {
                    break;
                }

                text_iterator.skip(*chars_to_skip);
                totally_skipped += *chars_to_skip;
                ++times;
            }

            if (repetition.inRange(times)) {
                return totally_skipped;
            }

            return std::nullopt;
        }

    private:
        [[nodiscard]] virtual auto scanIteration(const ForkedGenerator &text_iterator) const
            -> std::optional<std::size_t> = 0;
    };

    class DotItem : public std::unique_ptr<DotItemConcept>
    {
    public:
        using std::unique_ptr<DotItemConcept>::unique_ptr;

        template<std::derived_from<DotItemConcept> T>
        explicit DotItem(T dot_item)
          : std::unique_ptr<DotItemConcept>::unique_ptr(isl::makeUnique<T>(std::move(dot_item)))
        {}
    };
}// namespace ccl::lexer::dot_item
