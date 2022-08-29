#include <ccl/lex/analyzer_generator/ccll_parser.hpp>
#include <fmt/format.h>

namespace ccl::lex::parser
{
    auto CcllParser::parse() -> bool
    {
        while (true) {
            auto token = tokenizer.yield();
            auto token_id = token.getId();

            switch (token_id) {
            case GenToken::IDENTIFIER:
                token_stack.push(std::move(token));

                if (not parseDeclaration()) {
                    return false;
                }

                break;

            case GenToken::ANGLE_OPENING:
                if (not parseBlockDefinition()) {
                    return false;
                }

                break;

            case GenToken::NEW_LINE:
                break;

            case GenToken::EOI:
                return true;

            default:
                parsingError("identifier or angle opening (`[`)", token_id);
                return false;
            }
        }
    }

    auto CcllParser::parseDeclaration() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        switch (token_id) {
        case GenToken::COLUMN:
            return parseRuleDeclaration();

        case GenToken::ASSIGN:
            return parseDirectiveDeclaration();

        default:
            parsingError("column (`:`) or assign (`=`)", token_id);
            return false;
        }
    }

    auto CcllParser::parseRuleDeclaration() -> bool
    {
        while (true) {
            auto token = tokenizer.yield();
            auto token_id = token.getId();

            switch (token_id) {
            case GenToken::RAW_DATA:
                token_stack.push(std::move(token));
                completeRuleDeclaration();
                return true;

            case GenToken::NEW_LINE:
                break;

            default:
                parsingError("regular expression", token_id);
                return false;
            }
        }
    }

    auto CcllParser::parseDirectiveDeclaration() -> bool
    {
        while (true) {
            auto token = tokenizer.yield();
            auto token_id = token.getId();

            switch (token_id) {
            case GenToken::IDENTIFIER:
                token_stack.push(std::move(token));
                completeDirectiveDeclaration();
                return true;

            case GenToken::NEW_LINE:
                break;

            default:
                parsingError("identifier", token_id);
                return false;
            }
        }
    }

    auto CcllParser::completeRuleDeclaration() -> void
    {
        auto rule = token_stack.top();
        token_stack.pop();

        auto name = token_stack.top();
        token_stack.pop();

        rules.emplace_back(current_block, name.getValue(), rule.getValue());
    }

    auto CcllParser::completeDirectiveDeclaration() -> void
    {
        auto directive_value = token_stack.top();
        token_stack.pop();

        auto directive = token_stack.top();
        token_stack.pop();

        directives.emplace_back(directive.getValue(), directive_value.getValue());
    }

    auto CcllParser::parseBlockDefinition() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (GenToken::IDENTIFIER) {
            token_stack.push(std::move(token));
            return parseBlockEnding();
        }

        parsingError("identifier", token_id);
        return false;
    }

    auto CcllParser::parseBlockEnding() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (token_id == GenToken::ANGLE_CLOSING) {
            completeBlock();
            return true;
        }

        parsingError("angle closing (`]`)", token_id);
        return false;
    }

    auto CcllParser::completeBlock() -> void
    {
        auto block_name = token_stack.top();
        token_stack.pop();

        current_block = block_name.getValue();
    }

    auto CcllParser::parsingError(string_view expected_types, GenToken given_token) -> void
    {
        auto error_message = ::fmt::format(
            "expected {}, but got: {}", expected_types, GenTokenNames.at(given_token));
        tokenizer.throwError(error_message);
    }
}// namespace ccl::lex::parser
