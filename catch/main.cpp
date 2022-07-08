#include <string>
#include <string_view>

auto main() -> int
{
    std::string test{};
    test += std::string_view{ "Hello!" };

    return 0;
}
