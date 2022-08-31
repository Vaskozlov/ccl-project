#include <ccl/lex/analyzer_generator/ccll_parser.hpp>
#include <iostream>

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
                    recoverFromError();
                }

                break;

            case GenToken::CURLY_OPENING:
                if (not parseBlockDefinition()) {
                    recoverFromError();
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
            auto iterator_copy = tokenizer.getIterator();
            auto token = tokenizer.yield();
            auto token_id = token.getId();

            switch (token_id) {
            case GenToken::RULE_DECLARATION:
                iterator_copy.setEnd(token.getRepr().end());
                checkRule(iterator_copy);

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

        exceptRuleEnd();
    }

    auto CcllParser::completeDirectiveDeclaration() -> void
    {
        auto directive_value = token_stack.top();
        token_stack.pop();

        auto directive = token_stack.top();
        token_stack.pop();

        directives.emplace_back(directive.getValue(), directive_value.getValue());

        exceptRuleEnd();
    }

    auto CcllParser::checkRule(text::TextIterator &rule) -> void
    {
        try {
            auto container = dot_item::Container{ std::move(rule), 2, analysis_shared };
        } catch (const UnrecoverableError & /* unused */) {}
    }

    auto CcllParser::parseBlockDefinition() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (token_id == GenToken::IDENTIFIER) {
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

        if (token_id == GenToken::CURLY_CLOSING) {
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

        exceptRuleEnd();
    }

    auto CcllParser::exceptRuleEnd() -> void
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        switch (token_id) {
        case GenToken::EOI:
        case GenToken::NEW_LINE:
            break;
        default:
            parsingError("new line or end of input", token_id);
        }
    }

    auto CcllParser::recoverFromError() -> void
    {
        while (true) {
            auto token = tokenizer.yield();
            auto token_id = token.getId();

            switch (token_id) {
            case GenToken::EOI:
            case GenToken::NEW_LINE:
                return;

            default:
                break;
            }
        }
    }

    auto CcllParser::parsingError(
        string_view expected_types, GenToken given_token, string_view suggestion) -> void
    {
        auto error_message =
            fmt::format("expected {}, got {}", expected_types, GenTokenNames.at(given_token));
        tokenizer.throwException(ExceptionCriticality::PANIC, error_message, suggestion);
    }
}// namespace ccl::lex::parser