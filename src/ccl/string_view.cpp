#include <ccl/string_view.hpp>

namespace ccl
{
    template class BasicStringView<char>;
    template class BasicStringView<char8_t>;
    template class BasicStringView<char16_t>;
    template class BasicStringView<char32_t>;
    template class BasicStringView<wchar_t>;
}// namespace ccl
