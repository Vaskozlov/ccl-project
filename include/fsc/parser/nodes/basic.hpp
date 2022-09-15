#ifndef CCL_PROJECT_BASIC_HPP
#define CCL_PROJECT_BASIC_HPP

#include <fsc/fsc.hpp>

namespace fsc::ast
{
    struct BasicNode
    {
        BasicNode() = default;
        BasicNode(const BasicNode &) = default;
        BasicNode(BasicNode &&) noexcept = default;
        virtual ~BasicNode() = default;

        auto operator=(const BasicNode &) -> BasicNode & = default;
        auto operator=(BasicNode &&) noexcept -> BasicNode & = default;

        virtual auto print(const std::string &prefix = "", bool isLeft = false) const -> void = 0;
    };
}// namespace fsc

#endif /* CCL_PROJECT_BASIC_HPP */
