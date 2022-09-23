#ifndef CCL_PROJECT_PARAMS_LIST_HPP
#define CCL_PROJECT_PARAMS_LIST_HPP

#include <fsc/parser/nodes/basic.hpp>

namespace fsc::ast
{
    struct ParamsListNode : BasicNode
    {
        ParamsListNode() = default;

        auto print(const std::string &prefix, bool isLeft) const -> void override
        {
            fmt::print(
                "{}{}ParameterList\n", prefix, isLeft ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500");

            for (size_t i = 0; i < params.size(); i++) {
                params[i]->print(prefix + (isLeft ? "\u2502   " : "    "), i != params.size() - 1);
            }
        }

        std::vector<unique_ptr<BasicNode>> params;
    };
}// namespace fsc::ast

#endif /* CCL_PROJECT_PARAMS_LIST_HPP */