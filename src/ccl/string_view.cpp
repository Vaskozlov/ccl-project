#include <ccl/string_view.hpp>

namespace ccl
{
    template class BasicStringView<char>;
}// namespace ccl

auto fmt::formatter<ccl::string_view>::format(
    const ccl::string_view &str, format_context &ctx) const -> decltype(ctx.out())
{
    return formatter<std::string_view>::format(ccl::as<std::string_view>(str), ctx);
}
