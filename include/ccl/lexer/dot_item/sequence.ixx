export module ccl.lexer.dot_item:sequence;

export import isl;
export import ccl.text;
export import ccl.lexer.dot_item.core;

export namespace ccl::lexer::dot_item
{
    class Sequence final : public DotItemConcept
    {
    public:
        struct SequenceFlags
        {
            bool multiline = false;
            bool noEscaping = false;
        };

    private:
        using typename DotItemConcept::TextIterator;

        std::string sequenceValue{};
        isl::string_view starter{};
        isl::string_view ender{};

    public:
        [[nodiscard]] Sequence(
            SequenceFlags sequence_flags, isl::string_view sequence_starter,
            isl::string_view sequence_ender, TextIterator &rule_iterator, Id item_id = 0)
        : DotItemConcept{item_id, Flags{
            .sequenceIsMultiline=sequence_flags.multiline,
            .noEscapingSymbols=sequence_flags.noEscaping
        }}
        , starter{sequence_starter}
        , ender{sequence_ender}
        {
            static constexpr isl::StaticFlatmap<char32_t, char32_t, 2> special_symbols_for_sequence{
                {U'[', U'['}, {U']', U']'}};

            auto begin_iterator_state = rule_iterator;

            checkSequenceArguments(rule_iterator);
            skipStringDefinition(rule_iterator);

            while (true) {
                auto chr = U'\0';
                auto is_escaping = false;

                if (getFlags().noEscapingSymbols) {
                    chr = rule_iterator.advance();
                } else {
                    auto [escaping, character] =
                        rule_iterator.nextCharWithEscapingSymbols(std::span{
                            special_symbols_for_sequence.begin(),
                            special_symbols_for_sequence.end()});
                    is_escaping = escaping;
                    chr = character;
                }

                checkForUnexpectedEnd(begin_iterator_state, is_escaping, chr);

                if (isStringEnd(rule_iterator, is_escaping)) {
                    rule_iterator.skip(ender.size() - 1);
                    break;
                }

                isl::utf8::appendUtf32ToUtf8Container(sequenceValue, chr);
            }
        }

        [[nodiscard]] Sequence(
            SequenceFlags sequence_flags, isl::string_view sequence_begin_and_end,
            TextIterator &rule_iterator, Id item_id = 0)
          : Sequence{
                sequence_flags, sequence_begin_and_end, sequence_begin_and_end, rule_iterator,
                item_id}
        {}

        [[nodiscard]] auto getValue() noexcept -> std::string &
        {
            return sequenceValue;
        }

        [[nodiscard]] auto getValue() const noexcept -> const std::string &
        {
            return sequenceValue;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return sequenceValue.empty();
        }

    private:
        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> std::optional<std::size_t> override
        {
            isl::string_view future_text = text_iterator.getRemainingText();

            if (future_text.startsWith(sequenceValue) != isReversed()) [[unlikely]] {
                return isReversed() ? isl::utf8::size(future_text[0]) : sequenceValue.size();
            }

            return std::nullopt;
        }

        [[nodiscard]] auto
            isStringEnd(const TextIterator &rule_iterator, bool is_escaping) const -> bool
        {
            if (is_escaping) {
                return false;
            }

            auto text = rule_iterator.getRemainingWithCurrent();
            return text.startsWith(isl::as<isl::string_view>(ender));
        }

        auto skipStringDefinition(TextIterator &rule_iterator) const -> void
        {
            rule_iterator.skip(starter.size() - 1);
        }

        auto checkForUnexpectedEnd(
            TextIterator &rule_iterator, bool is_escaping, char32_t chr) const -> void
        {
            if (is_escaping) {
                return;
            }

            if (text::isEoF(chr)) {
                throwUnterminatedString(rule_iterator, "unterminated sequence");
            }

            if ('\n' == chr && !getFlags().sequenceIsMultiline) [[unlikely]] {
                constexpr auto message =
                    "new line is reached, but sequence has not been terminated";
                auto suggestion =
                    std::format("use multiline sequence or close it with `{}`", ender);

                throwUnterminatedString(rule_iterator, message, suggestion);
            }
        }

        auto checkSequenceArguments(TextIterator &rule_iterator) const -> void
        {
            auto text = rule_iterator.getRemainingWithCurrent();

            if (starter.empty()) [[unlikely]] {
                throwEmptyStringBegin(rule_iterator);
            }

            if (ender.empty()) [[unlikely]] {
                throwEmptyStringEnd(rule_iterator);
            }

            if (!text.startsWith(starter)) [[unlikely]] {
                throwStringBeginException(rule_iterator);
            }
        }

        [[noreturn]] auto throwStringBeginException(TextIterator &rule_iterator) const -> void
        {
            auto message = std::format("string literal must begin with {}", starter);

            rule_iterator.throwPanicError(text::AnalysisStage::LEXICAL_ANALYSIS, message);
            throw UnrecoverableError{"unrecoverable error in SequenceType"};
        }

        [[noreturn]] static auto throwEmptyStringEnd(TextIterator &rule_iterator) -> void
        {
            rule_iterator.throwPanicError(
                text::AnalysisStage::LEXICAL_ANALYSIS, "sequence item end cannot be empty");
            throw UnrecoverableError{"unrecoverable error in SequenceType"};
        }

        [[noreturn]] static auto throwEmptyStringBegin(TextIterator &rule_iterator) -> void
        {
            rule_iterator.throwPanicError(
                text::AnalysisStage::LEXICAL_ANALYSIS, "sequence item begin cannot be empty");
            throw UnrecoverableError{"unrecoverable error in SequenceType"};
        }

        [[noreturn]] static auto throwUnterminatedString(
            TextIterator &rule_iterator,
            isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            rule_iterator.throwPanicError(
                text::AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
            throw UnrecoverableError{"unrecoverable error in SequenceType"};
        }
    };
}// namespace ccl::lexer::dot_item
