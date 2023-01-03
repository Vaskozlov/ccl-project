#ifndef CCL_PROJECT_BASIC_CODEGEN_HPP
#define CCL_PROJECT_BASIC_CODEGEN_HPP

#include <ccl/const_string.hpp>
#include <ccl/string_view.hpp>
#include <fmt/core.h>
#include <iterator>

namespace ccl::codegen
{
    class BasicCodeGenerator
    {
    public:
        struct CCL_TRIVIAL_ABI PushScope : std::true_type
        {
        };

        struct CCL_TRIVIAL_ABI PopScope : std::false_type
        {
        };

        struct CCL_TRIVIAL_ABI Endl : std::true_type
        {
        };

        struct CCL_TRIVIAL_ABI ScopeSize
        {
            size_t size = 4;
        };

    private:
        std::string generatedCode{};
        size_t scopesCounter = 0;
        size_t scopeSize = 4;

    public:
        BasicCodeGenerator() = default;

        [[nodiscard]] auto getCode() const noexcept -> const std::string &
        {
            return generatedCode;
        }

        auto operator<<(ScopeSize scope_size) -> BasicCodeGenerator &;
        auto operator<<(PushScope /* unused */) -> BasicCodeGenerator &;
        auto operator<<(PopScope /* unused */) -> BasicCodeGenerator &;

        auto operator<<(char character) -> BasicCodeGenerator &;
        auto operator<<(string_view string) -> BasicCodeGenerator &;
        auto operator<<(std::string_view string) -> BasicCodeGenerator &;
        auto operator<<(const std::string &string) -> BasicCodeGenerator &;

        template<std::integral Int>
        auto operator<<(Int value) -> BasicCodeGenerator &
        {
            fmt::format_to(std::back_inserter(generatedCode), "{}", value);
            return *this;
        }

        template<std::floating_point Float>
        auto operator<<(Float value) -> BasicCodeGenerator &
        {
            fmt::format_to(std::back_inserter(generatedCode), "{}", value);
            return *this;
        }

        auto operator<<(Endl /* unused */) -> BasicCodeGenerator &
        {
            newLine();
            return *this;
        }

        template<size_t Size>
        auto operator<<(const ConstString<Size> &string) -> BasicCodeGenerator &
        {
            return *this << as<string_view>(string);
        }

        template<size_t N>
        auto operator<<(const char (&character_array)[N]) -> BasicCodeGenerator &// NOLINT
        {
            return *this << std::string_view{
                       std::forward<const char(&)[N]>(character_array)};// NOLINT
        }

    private:
        auto newLine() -> void;
        auto addScope(size_t scopes_count) -> void;
    };

    inline auto setScopeSize(size_t size) noexcept -> BasicCodeGenerator::ScopeSize
    {
        return {.size = size};
    }

    constexpr inline auto endl = BasicCodeGenerator::Endl{};
    constexpr inline auto push_scope = BasicCodeGenerator::PushScope{};
    constexpr inline auto pop_scope = BasicCodeGenerator::PopScope{};

}// namespace ccl::codegen

#endif /* CCL_PROJECT_BASIC_CODEGEN_HPP */
