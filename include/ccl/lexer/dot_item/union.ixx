export module ccl.lexer.dot_item:union_item;

export import isl;
export import ccl.lexer.dot_item.core;

export namespace ccl::lexer::dot_item
{
    class Union final : public DotItemConcept
    {
    private:
        isl::UtfSet bitset{};

    public:
        using typename DotItemConcept::TextIterator;

        [[nodiscard]] explicit Union(TextIterator &rule_iterator, Id rule_id = 0)
          : DotItemConcept{rule_id}
        {
            static constexpr isl::StaticFlatmap<char32_t, char32_t, 3> special_symbols = {
                {U'[', U'['}, {U']', U']'}, {U'-', U'-'}};

            auto is_range = false;
            auto previous_chr = U'\0';
            auto begin_iterator_state = rule_iterator;

            checkUnionBegin(rule_iterator);

            while (true) {
                auto [is_escaping, chr] = rule_iterator.nextCharWithEscapingSymbols(
                    std::span{special_symbols.begin(), special_symbols.end()});

                checkForUnexpectedEnd(begin_iterator_state, is_escaping, chr);

                if (isUnionEnd(is_escaping, chr)) {
                    break;
                }

                if (isRange(is_escaping, chr)) {
                    is_range = true;
                    continue;
                }

                addCharactersToTheBitset(is_range, previous_chr, chr);
                previous_chr = chr;
            }

            checkForClosedRange(rule_iterator, is_range);
        }

        [[nodiscard]] auto getBitset() const noexcept -> const isl::UtfSet &
        {
            return bitset;
        }

        [[nodiscard]] auto empty() const noexcept -> bool override
        {
            return bitset.empty();
        }

    private:
        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> std::optional<std::size_t> override
        {
            if (bitset.at(text_iterator.futureChar()) != isReversed()) [[unlikely]] {
                return isl::utf8::size(text_iterator.getNextCarriageValue());
            }

            return std::nullopt;
        }

        [[nodiscard]] static auto isRange(bool is_escaping, char32_t chr) noexcept -> bool
        {
            if (is_escaping) {
                return false;
            }

            return chr == U'-';
        }

        [[nodiscard]] static auto isUnionEnd(bool is_escaping, char32_t chr) noexcept -> bool
        {
            return !is_escaping && chr == U']';
        }

        auto addCharactersToTheBitset(bool &is_range, char32_t previous_chr, char32_t chr) -> void
        {
            if (is_range) {
                bitset.set({previous_chr, chr + 1}, true);
                is_range = false;
            } else {
                bitset.set(chr, true);
            }
        }

        static auto checkForUnexpectedEnd(
            TextIterator &rule_iterator, bool is_escaping, char32_t chr) -> void
        {
            if (!is_escaping && text::isEoF(chr)) {
                throwUnterminatedUnion(rule_iterator);
            }
        }

        static auto checkUnionBegin(TextIterator &rule_iterator) -> void
        {
            if (U'[' != rule_iterator.getCurrentChar()) {
                throwUnionBeginException(rule_iterator);
            }
        }

        static auto checkForClosedRange(TextIterator &rule_iterator, bool is_ranged_opened) -> void
        {
            if (is_ranged_opened) {
                throwUnterminatedRangeException(rule_iterator);
            }
        }

        [[noreturn]] static auto throwUnterminatedUnion(TextIterator &rule_iterator) -> void
        {
            rule_iterator.throwPanicError(
                text::AnalysisStage::LEXICAL_ANALYSIS, "unterminated union item");
            throw UnrecoverableError{"unrecoverable error in Union"};
        }

        [[noreturn]] static auto throwUnionBeginException(TextIterator &rule_iterator) -> void
        {
            rule_iterator.throwPanicError(
                text::AnalysisStage::LEXICAL_ANALYSIS, "unterminated range");
            throw UnrecoverableError{"unrecoverable error in Union"};
        }

        [[noreturn]] static auto
            throwUnterminatedRangeException(TextIterator &rule_iterator) -> void
        {
            auto buffer = std::string{};
            isl::utf8::appendUtf32ToUtf8Container(buffer, rule_iterator.getCurrentChar());

            auto message = std::format(
                "expected `[` at the beginning of union item declaration, got {}", buffer);

            rule_iterator.throwPanicError(text::AnalysisStage::LEXICAL_ANALYSIS, message);
            throw UnrecoverableError{"unrecoverable error in Union"};
        }
    };
}// namespace ccl::lexer::dot_item
