#include <ccl/lex/analyzer_generator/ccll_parser.hpp>
#include <ccl/lex/dot_item/sequence.hpp>
#include <iostream>

namespace ccl::lex::parser
{
    auto CcllParser::parse() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        switch (token_id) {
        case GenToken::IDENTIFIER:
            token_stack.push(std::move(token));

            if (!parseDeclaration()) {
                recoverFromError();
            }

            return parse();

        case GenToken::CURLY_OPENING:
            if (!parseBlockDefinition()) {
                recoverFromError();
            }

            return parse();

        case GenToken::NEW_LINE:
            return parse();

        case GenToken::EOI:
            return true;

        default:
            parsingError(
                "unrecognizable action",
                "use { `identifier` } to declare group of items or `identifier` = identifier | "
                "'$STRING' to declare variable");
            recoverFromError();
            return parse();
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
            parsingError(
                "bad rule or variable declaration declaration",
                "use `:` to declare rule or `=` to declare variable");
            return false;
        }
    }

    auto CcllParser::parseRuleDeclaration() -> bool
    {
        auto iterator_copy = tokenizer.getIterator();
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (GenToken::RULE_DECLARATION == token_id) {
            iterator_copy.setEnd(token.getRepr().end());
            checkRule(iterator_copy);

            token_stack.push(std::move(token));
            completeRuleDeclaration();

            return true;
        }

        parsingError("expected rule declaration");
        return false;
    }

    auto CcllParser::parseDirectiveDeclaration() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (GenToken::IDENTIFIER == token_id || GenToken::STRING == token_id) {
            token_stack.push(std::move(token));
            completeDirectiveDeclaration();
            return true;
        }

        parsingError("expected identifier or string ( '$CHARACTERS' )");
        return false;
    }

    auto CcllParser::completeRuleDeclaration() -> void
    {
        auto rule = token_stack.top();
        token_stack.pop();

        auto name = token_stack.top();
        token_stack.pop();

        rules.emplace_back(current_block, blocks[current_block], name.getRepr(), rule.getRepr());

        expectRuleEnd();
    }

    auto CcllParser::completeDirectiveDeclaration() -> void
    {
        auto directive_value = token_stack.top();
        token_stack.pop();

        auto directive = token_stack.top();
        token_stack.pop();

        auto directive_repr = directive_value.getRepr();

        if (GenToken::STRING == directive_value.getId()) {
            auto string_part_of_repr = directive_repr.substr(1, directive_repr.size() - 2);
            directives.emplace(directive.getRepr(), string_part_of_repr);
        } else {
            directives.emplace(directive.getRepr(), directive_repr);
        }

        expectRuleEnd();
    }

    auto CcllParser::checkRule(text::TextIterator &rule) -> void
    {
        try {
            auto container = dot_item::Container{rule, special_items, 2, true};
        } catch (const UnrecoverableError & /* unused */) {}
    }

    auto CcllParser::parseBlockDefinition() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (GenToken::IDENTIFIER == token_id) {
            token_stack.push(std::move(token));
            return parseBlockEnding();
        }

        parsingError("expected identifier after `{`", "add identifier");
        return false;
    }

    auto CcllParser::parseBlockEnding() -> bool
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        if (GenToken::CURLY_CLOSING == token_id) {
            completeBlock();
            return true;
        }

        parsingError("expected `}` after identifier", "add `}`");
        return false;
    }

    auto CcllParser::completeBlock() -> void
    {
        auto block_name = token_stack.top();
        token_stack.pop();

        current_block = block_name.getRepr();

        if (!blocks.contains(current_block)) {
            blocks.insert({current_block, {as<u16>(last_block_id++), 0}});
        }

        expectRuleEnd();
    }

    auto CcllParser::expectRuleEnd() -> void
    {
        auto token = tokenizer.yield();
        auto token_id = token.getId();

        switch (token_id) {
        case GenToken::EOI:
        case GenToken::NEW_LINE:
            break;
        default:
            parsingError("expected new line of end of input");
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

    auto CcllParser::parsingError(string_view message, string_view suggestion) -> void
    {
        tokenizer.throwException(ExceptionCriticality::PANIC, message, suggestion);
    }
}// namespace ccl::lex::parser
