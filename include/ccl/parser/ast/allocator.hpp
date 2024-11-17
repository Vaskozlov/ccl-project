#ifndef CCL_PROJECT_PARSER_AST_ALLOCATOR_HPP
#define CCL_PROJECT_PARSER_AST_ALLOCATOR_HPP

#include <ccl/parser/ast/terminal.hpp>
#include <isl/memory.hpp>

namespace ccl::parser::ast
{
    namespace detail
    {
        using SharedPtrFrame = isl::SharedPtrFrame<256, 16>;

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        extern isl::FixedSizeAllocator<sizeof(SharedPtrFrame), alignof(SharedPtrFrame)>
            NodeAllocator;
    }// namespace detail

    template<std::derived_from<Node> T = Node>
    using SharedNode =
        isl::SharedPtr<T, detail::SharedPtrFrame, std::addressof(detail::NodeAllocator)>;
}// namespace ccl::parser::ast

#endif /* CCL_PROJECT_PARSER_AST_ALLOCATOR_HPP */
