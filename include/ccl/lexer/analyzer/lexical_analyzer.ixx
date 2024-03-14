export module ccl.lexer.analyzer:lexical_analyzer;

export import :tokenizer;

export namespace ccl::lexer
{
    class LexicalAnalyzer
    {
    private:
        struct Rule
        {
            Id id{};
            isl::string_view repr{};
        };

        std::vector<dot_item::Container> items{};
        dot_item::SpecialItems specialItems{};
        std::vector<Id> ignoredIds{};
        std::string skippedCharacters{};
        handler::ExceptionHandler &exceptionHandler;// NOLINT

    public:
        [[nodiscard]] LexicalAnalyzer(
            handler::ExceptionHandler &exception_handler, const std::initializer_list<Rule> &rules,
            std::string_view filename = {}, std::vector<Id> ignored_ids = {})
          : ignoredIds{std::move(ignored_ids)}
          , exceptionHandler{exception_handler}
        {
            for (const Rule &rule : rules) {
                createContainer(rule.repr, rule.id, filename);
            }
        }

        [[nodiscard]] auto getIgnoredIds() const -> const std::vector<Id> &
        {
            return ignoredIds;
        }

        [[nodiscard]] auto
            getTokenizer(isl::string_view text, std::string_view filename = {}) -> Tokenizer
        {
            return getTokenizer(text, filename, exceptionHandler);
        }

        [[nodiscard]] auto getTokenizer(
            isl::string_view text, std::string_view filename,
            handler::ExceptionHandler &handler) -> Tokenizer
        {
            return {items, specialItems, ignoredIds, skippedCharacters, handler, text, filename};
        }

    private:
        auto createContainer(isl::string_view rule, Id id, std::string_view filename) -> void
        {
            auto container = dot_item::Container{
                text::TextIterator{rule, exceptionHandler, filename}, specialItems, id, true};

            if (container.isSpecial()) {
                specialItems.specialItems.emplace_back(std::move(container));
            } else if (!container.empty()) {
                items.emplace_back(std::move(container));
            }
        }
    };
}// namespace ccl::lexer
