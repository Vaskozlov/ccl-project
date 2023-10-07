#include <ccl/lex/analyzer_generator/ccll_parser.hpp>
#include <ccll/ccll.hpp>

namespace ccl::lexer::parser
{
    CcllParser::Rule::Rule(
        BlockInfo &block_info, string_view rule_name, string_view rule_definition)
      : name{rule_name}
      , definition{rule_definition}
      , blockId{block_info.blockId}
      , id{block_info.lastId++}
    {}

    CcllParser::CcllParser(Tokenizer &input_tokenizer)
      : tokenizer{input_tokenizer}
    {}

    auto CcllParser::parse() -> bool
    {
        using namespace ccll;

        while (const Token &token = tokenizer.yield()) {
            switch (token.getId()) {
            case GROUP_DECLARATION:
                completeGroup(token);
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

    auto CcllParser::completeGroup(const Token &token) -> void
    {
        const ContainerOf<string_view> auto &prefixes = token.getPrefixes();
        auto group_name = prefixes.at(1);

        currentBlock = group_name;

        if (!blocks.contains(currentBlock)) {
            const auto block_id = as<u16>(lastBlockId);
            ++lastBlockId;

            blocks.try_emplace(blocks.end(), currentBlock, BlockInfo{block_id, 0});
        }
    }

    auto CcllParser::completeRule(const Token &token) -> void
    {
        const ContainerOf<string_view> auto &prefixes = token.getPrefixes();
        const ContainerOf<string_view> auto &postfixes = token.getPostfixes();

        auto name = prefixes.at(0);
        auto rule = postfixes.at(0);

        checkRule(token);
        rules.emplace_back(blocks[currentBlock], name, rule);
    }

    auto CcllParser::completeDirective(const Token &token) -> void
    {
        const ContainerOf<string_view> auto &prefixes = token.getPrefixes();
        const ContainerOf<string_view> auto &postfixes = token.getPostfixes();

        auto name = prefixes.at(0);
        auto value = postfixes.at(0);

        // value is represented as string, so we need to remove " from both sides
        value = value.substr(1, value.size() - 2);
        directives.try_emplace(name, value);
    }

    auto CcllParser::checkRule(const Token &token) -> void
    {
        const string_view line_repr = token.getInlineRepr();
        const text::Location &location = token.getLocation();

        auto text_iterator = text::TextIterator{
            line_repr, tokenizer.getHandler(),
            text::Location{
                location.getFilename(), location.getLine(), location.getColumn() - 1,
                location.getRealColumn() - 1}};

        // NOLINTNEXTLINE : is guaranteed by lexical analyzer rule
        text_iterator.skip(line_repr.find(':').value() + 1);

        [[maybe_unused]] auto just_checking_rule =
            dot_item::Container{text_iterator, specialItems, 2, true};
    }

    auto CcllParser::parsingError(string_view message, string_view suggestion) -> void
    {
        tokenizer.throwExceptionToHandler(ExceptionCriticality::PANIC, message, suggestion);
    }
}// namespace ccl::lex::parser
