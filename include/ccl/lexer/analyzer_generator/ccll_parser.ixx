export module ccl.lexer.analyzer_generator:ccll_parser;

export import ccll;
export import ccl.lexer.analyzer;

export namespace ccl::lexer::generator::parser
{
    class CcllParser
    {
    public:
        struct BlockInfo
        {
            u16 blockId{};
            u16 lastId{};
        };

        class Rule
        {
        public:
            isl::string_view name;
            isl::string_view definition;
            u16 blockId{};
            u16 id{};

            Rule() = default;

            Rule(
                BlockInfo &block_info, isl::string_view rule_name,
                isl::string_view rule_definition);
        };

    private:
        class CompareTokenAsString
        {
        public:
            [[nodiscard]] auto operator()(const Token &lhs, const Token &rhs) const -> bool
            {
                return lhs.getRepr() < rhs.getRepr();
            }
        };

        std::vector<Rule> rules;
        std::stack<Token> tokenStack;
        std::map<Token, isl::string_view, CompareTokenAsString> directives;
        std::map<isl::string_view, BlockInfo> blocks{{"NONE", {0, ReservedTokenMaxValue + 1}}};
        dot_item::SpecialItems specialItems{};
        isl::string_view currentBlock = "NONE";
        Tokenizer &tokenizer;
        std::size_t previousBlockId{1};

    public:
        explicit CcllParser(Tokenizer &input_tokenizer);

        [[nodiscard]] auto getRules() const -> const std::vector<Rule> &
        {
            return rules;
        }

        [[nodiscard]] auto getBlocks() const -> const std::map<isl::string_view, BlockInfo> &
        {
            return blocks;
        }

        [[nodiscard]] auto
            getDirectives() const -> const std::map<Token, isl::string_view, CompareTokenAsString> &
        {
            return directives;
        }

        auto parse() -> bool;

    private:
        auto completeRule(const Token &token) -> void;
        auto completeDirective(const Token &token) -> void;
        auto completeGroup(const Token &token) -> void;

        auto checkRule(const Token &token) -> void;

        auto parsingError(isl::string_view message, isl::string_view suggestion = {}) -> void;
    };
}// namespace ccl::lexer::generator::parser
