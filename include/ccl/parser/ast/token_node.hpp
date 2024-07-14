#ifndef CCL_PROJECT_TOKEN_NODE_HPP
#define CCL_PROJECT_TOKEN_NODE_HPP

#include <ccl/parser/ast/node.hpp>

namespace ccl::parser::ast
{
    class TokenNode : public Node
    {
    private:
        lexer::Token token;

    public:
        explicit TokenNode(Id node_type_id, lexer::Token l_token)
          : Node{node_type_id}
          , token{std::move(l_token)}
        {}

        auto print(
            const std::string &prefix, bool is_left,
            std::function<isl::string_view(Id)> id_converter) const -> void override;
    };
}// namespace ccl::parser::ast

#endif /* CCL_PROJECT_TOKEN_NODE_HPP */
