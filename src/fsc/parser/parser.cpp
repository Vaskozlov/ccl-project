#include <fsc/parser/parser.hpp>
#include <unordered_map>

namespace fsc
{
    // NOLINTBEGIN
    std::unordered_map<size_t, size_t> BinaryTokenPrecedence{
        { FscLexerToken::ADDITION, 10 },
        { FscLexerToken::SUBTRACTION, 10 },
        { FscLexerToken::MULTIPLICATION, 20 },
        { FscLexerToken::DIVISION, 20 },
    };
    // NOLINTEND

    static inline auto isBinaryOperation(const Token &token) -> bool
    {
        return BinaryTokenPrecedence.contains(token.getId());
    }

    static inline auto getBinaryTokenPrecedence(const Token &token) -> size_t
    {
        if (isBinaryOperation(token)) {
            return BinaryTokenPrecedence.at(token.getId());
        }

        return 0;
    }

    auto Parser::parse() -> void
    {
        parseFunctionDeclaration()->print();

        /* while (true) {
             auto future_token = tokenizer.futureToken();

             switch (future_token.getId()) {
             case FscLexerToken::EOI:
                 return;

             case FscLexerToken::FUNC:
                 break;

             case FscLexerToken::VAR:
                 break;

             case FscLexerToken::CLASS:
                 break;

             default:
                 badTokenError();
                 break;
             }
         }*/
    }

    auto Parser::parseFunctionDeclaration() -> unique_ptr<ast::BasicNode>
    {
        [[maybe_unused]] auto &function_keyword = tokenizer.yield();
        auto function_declaration = make_unique<ast::FunctionDeclarationNode>();

        // Parse function name
        auto &token = tokenizer.yield();
        if (token.getId() != FscLexerToken::IDENTIFIER) {
            badTokenError();
        }

        function_declaration->name = token;

        auto &open_parenthesis = tokenizer.yield();

        if (open_parenthesis.getId() != FscLexerToken::LEFT_PARENTHESIS) {
            badTokenError();
        }

        function_declaration->parameters = parseParameters();

        auto &close_parenthesis = tokenizer.yield();

        if (close_parenthesis.getId() != FscLexerToken::RIGHT_PARENTHESIS) {
            badTokenError();
        }

        auto &arrow = tokenizer.futureToken();

        if (arrow.getId() != FscLexerToken::ARROW) {
            return function_declaration;
        }

        tokenizer.yield();

        auto &return_type = tokenizer.yield();

        if (return_type.getId() != FscLexerToken::IDENTIFIER) {
            badTokenError();
        }

        function_declaration->return_type = return_type;
        function_declaration->body = parseExpression();

        return function_declaration;
    }

    auto Parser::parseParameters() -> unique_ptr<ast::ParamsListNode>
    {
        auto parameters = make_unique<ast::ParamsListNode>();
        auto has_parameter = false;

        while (true) {
            if (has_parameter) {
                auto future_token = tokenizer.futureToken();

                if (future_token.getId() == FscLexerToken::COMMA) {
                    tokenizer.yield();
                    continue;
                }

                break;
            }

            auto expression = parseExpression();

            if (not expression) {
                break;
            }

            has_parameter = true;
            parameters->params.push_back(std::move(expression));
        }

        return parameters;
    }

    auto Parser::parseExpression() -> unique_ptr<ast::BasicNode>
    {
        auto lhs = parseTerm();

        if (not lhs) {
            return nullptr;
        }

        while (isBinaryOperation(tokenizer.futureToken())) {
            lhs = parseRhsExpression(0, std::move(lhs));
        }

        return lhs;
    }

    auto Parser::parseRhsExpression(size_t precedence, unique_ptr<ast::BasicNode> lhs)
        -> unique_ptr<ast::BasicNode>
    {
        while (true) {
            auto binary_operation_token = tokenizer.futureToken();

            if (not isBinaryOperation(binary_operation_token)) {
                return lhs;
            }

            auto binary_operation_precedence = getBinaryTokenPrecedence(binary_operation_token);

            if (binary_operation_precedence < precedence) {
                return lhs;
            }

            binary_operation_token = tokenizer.yield();

            auto rhs = parseTerm();

            if (not rhs) {
                return lhs;// TODO: check expression end
            }

            auto &future_token = tokenizer.futureToken();
            auto future_token_precedence = getBinaryTokenPrecedence(future_token);

            if (binary_operation_precedence < future_token_precedence) {
                rhs = parseRhsExpression(binary_operation_precedence + 1, std::move(rhs));
            }

            lhs = make_unique<ast::BinaryExpressionNode>(
                binary_operation_token, std::move(lhs), std::move(rhs));
        }
    }

    auto Parser::parseTerm() -> unique_ptr<ast::BasicNode>
    {
        auto &token = tokenizer.futureToken();

        switch (token.getId()) {
        case FscLexerToken::NUMBER:
            tokenizer.yield();
            return make_unique<ast::TermNode>(token);

        case FscLexerToken::IDENTIFIER:
            tokenizer.yield();
            return termLookAhead();

        case FscLexerToken::SUBTRACTION:
            tokenizer.yield();
            return make_unique<ast::NegateNode>(parseTerm());

        default:
            return nullptr;
        }
    }

    auto Parser::termLookAhead() -> unique_ptr<ast::BasicNode>
    {
        auto &token = tokenizer.getCurrentToken();
        auto &future_token = tokenizer.futureToken();

        switch (future_token.getId()) {
        case FscLexerToken::LEFT_PARENTHESIS:
            return nullptr;

        case FscLexerToken::ANGLE_OPENING:
            return nullptr;

        default:
            return make_unique<ast::TermNode>(token);
        }
    }

    auto Parser::badTokenError() -> void
    {
        tokenizer.throwException(ccl::ExceptionCriticality::PANIC, "bad token");
    }
}// namespace fsc
