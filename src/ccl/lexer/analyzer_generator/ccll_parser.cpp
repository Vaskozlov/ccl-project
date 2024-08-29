#include <ccl/lexer/analyzer_generator/ccll_parser.hpp>
#include <ccll/ccll.hpp>

namespace ccl::lexer::parser
{
    static auto EmptyLexicalAnalyzer = LexicalAnalyzer(ExceptionHandler::instance(), {});

    CcllParser::Rule::Rule(
        SmallId rule_id, isl::string_view rule_name, isl::string_view rule_definition)
      : name{rule_name}
      , definition{rule_definition}
      , id{rule_id}
    {}

    CcllParser::CcllParser(Tokenizer &input_tokenizer)
      : tokenizer{input_tokenizer}
    {}

    auto CcllParser::parse() -> bool
    {
        using namespace ccll;

        while (const Token &token = tokenizer.yield()) {
            switch (isl::as<CcllAnalyzerToken>(token.getId())) {
            case GROUP_DECLARATION:
                break;

            case BAD_GROUP_DECLARATION_ONLY_BRACKET:
                parsingError("group name expected");
                break;

            case BAD_GROUP_DECLARATION_BRACKET_AND_NAME:
                parsingError("group end expected", "insert ]");
                break;

            case BAD_GROUP_DECLARATION_EMPTY_NAME:
                parsingError("group name can not be empty");
                break;

            case DIRECTIVE:
                completeDirective(token);
                break;

            case BAD_DIRECTIVE_DECLARATION:
                parsingError("directive value expected");
                break;

            case RULE:
                completeRule(token);
                break;

            case BAD_RULE_DECLARATION:
                parsingError("rule definition expected");
                break;

            case BAD_RULE_OR_DIRECTIVE_DECLARATION:
                parsingError("rule or directive declaration expected");
                break;

            case BAD_GROUP_NO_OPEN_BRACKET:
                parsingError("unable to match ] to close group declaration");
                break;

            case COMMENT:
                break;

            default:
                parsingError("unknown token");
                break;
            }
        }

        return {};
    }


    auto CcllParser::completeRule(const Token &token) -> void
    {
        const auto &extracted_parts = token.getExtractedParts();

        auto name = extracted_parts.at(0);
        auto rule = extracted_parts.at(1);

        checkRule(token);
        rules.emplace_back(idGenerator.next(), name, rule);
    }

    auto CcllParser::completeDirective(const Token &token) -> void
    {
        const auto &extracted_parts = token.getExtractedParts();

        const auto &name = extracted_parts.at(0);
        auto value = extracted_parts.at(1);

        // value is represented as string, so we need to remove " from both sides
        value = value.substr(1, value.size() - 2);
        directives.try_emplace(token.cut(0, name.size()), value);
    }

    auto CcllParser::checkRule(const Token &token) -> void
    {
        const auto line_repr = token.getInlineRepr();
        const auto &location = token.getLocation();

        auto text_iterator = text::TextIterator{
            line_repr, tokenizer.getHandler(),
            text::Location{
                .line = location.line,
                .column = location.column - 1,
            }};

        // NOLINTNEXTLINE : is guaranteed by lexical analyzer rule
        text_iterator.skip(line_repr.find(':').value() + 1);
        std::ignore = rule::Container{EmptyLexicalAnalyzer, text_iterator, true};
    }

    auto CcllParser::parsingError(isl::string_view message, isl::string_view suggestion) -> void
    {
        tokenizer.throwExceptionToHandler(ExceptionCriticality::PANIC, message, suggestion);
    }
}// namespace ccl::lexer::parser
