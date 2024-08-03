#ifndef CCL_PROJECT_STRING_NODE_HPP
#define CCL_PROJECT_STRING_NODE_HPP

#include <ccl/parser/ast/node.hpp>

namespace ccl::parser::ast
{
    class StringNode : public Node
    {
    protected:
        std::string repr;

    public:
        explicit StringNode(SmallId node_type_id, isl::string_view value_repr)
          : Node{node_type_id}
          , repr{value_repr}
        {}

        auto print(
            const std::string &prefix, bool is_left,
            std::function<std::string(SmallId)> id_converter) const -> void override;
    };
}// namespace ccl::parser::ast

#endif /* CCL_PROJECT_STRING_NODE_HPP */
