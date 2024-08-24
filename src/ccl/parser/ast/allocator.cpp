#include <ccl/parser/ast/allocator.hpp>

namespace ccl::parser::ast::detail
{
    isl::FixedSizeAllocator<sizeof(SharedPtrFrame), alignof(SharedPtrFrame)> NodeAllocator{};
}
