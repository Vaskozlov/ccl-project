#ifndef CCL_PROJECT_TOKEN_NODE_HPP
#define CCL_PROJECT_TOKEN_NODE_HPP

#include <ccl/lexer/token.hpp>
#include <ccl/parser/ast/node.hpp>

namespace ccl::parser::ast
{
    class Terminal final : public Node
    {
    private:
        lexer::Token token;

    public:
        explicit Terminal(const SmallId node_type_id)
          : Node{node_type_id}
        {}

        explicit Terminal(lexer::Token l_token)
          : Node{l_token.getId()}
          , token{std::move(l_token)}
        {}

        explicit Terminal(const SmallId node_type_id, lexer::Token l_token)
          : Node{node_type_id}
          , token{std::move(l_token)}
        {}

        auto setToken(lexer::Token l_token) -> void
        {
            token = std::move(l_token);
        }

        [[nodiscard]] auto getToken() const noexcept -> const lexer::Token &
        {
            return token;
        }

        auto print(
            const std::string &prefix, bool is_left,
            const std::function<std::string(SmallId)> &id_converter) const -> void override;

        auto getChildrenNodes() const -> isl::SmallFunction<SharedNode<>()> override;
    };
}// namespace ccl::parser::ast

#endif /* CCL_PROJECT_TOKEN_NODE_HPP */
