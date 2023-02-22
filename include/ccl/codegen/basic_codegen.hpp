#ifndef CCL_PROJECT_BASIC_CODEGEN_HPP
#define CCL_PROJECT_BASIC_CODEGEN_HPP

#include <ccl/const_string.hpp>
#include <ccl/string_view.hpp>
#include <map>

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

        struct CCL_TRIVIAL_ABI StreamId
        {
            Id streamId = 0;
        };

    private:
        std::map<Id, std::string> generatedCode{};
        Id streamId = 0;
        size_t scopesCounter = 0;
        size_t scopeSize = 4;

    public:
        BasicCodeGenerator() = default;

        [[nodiscard]] auto getCode() const noexcept -> std::string;

        [[nodiscard]] auto getBackInserter() noexcept -> auto
        {
            return std::back_inserter(getCurrentStream());
        }

        auto operator<<(ScopeSize scope_size) -> BasicCodeGenerator &;
        auto operator<<(PushScope /* unused */) -> BasicCodeGenerator &;
        auto operator<<(PopScope /* unused */) -> BasicCodeGenerator &;
        auto operator<<(StreamId stream_id) -> BasicCodeGenerator &;

        auto operator<<(char character) -> BasicCodeGenerator &;
        auto operator<<(string_view string) -> BasicCodeGenerator &;
        auto operator<<(std::string_view string) -> BasicCodeGenerator &;
        auto operator<<(const std::string &string) -> BasicCodeGenerator &;

        template<typename Numeric>
        auto operator<<(Numeric value) -> BasicCodeGenerator &
            requires std::floating_point<Numeric> || std::integral<Numeric>
        {
            return *this << std::string_view{fmt::to_string(value)};
        }

        template<typename T>
        auto operator<<(T &&value) -> BasicCodeGenerator &
            requires(!(std::floating_point<T> || std::integral<T>))
        {
            fmt::format_to(getBackInserter(), "{}", std::forward<T>(value));
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
        [[nodiscard]] auto getCurrentStream() noexcept -> std::string &
        {
            return generatedCode[streamId];
        }

        [[nodiscard]] auto getCurrentStream() const noexcept -> const std::string &
        {
            return generatedCode.at(streamId);
        }

        auto newLine() -> void;
        auto addScope(size_t scopes_count) -> void;
    };

    inline auto setScopeSize(size_t size) noexcept -> BasicCodeGenerator::ScopeSize
    {
        return {.size = size};
    }

    inline auto setStream(Id stream_id) noexcept -> BasicCodeGenerator::StreamId
    {
        return {.streamId = stream_id};
    }

    constexpr inline auto endl = BasicCodeGenerator::Endl{};
    constexpr inline auto push_scope = BasicCodeGenerator::PushScope{};
    constexpr inline auto pop_scope = BasicCodeGenerator::PopScope{};

}// namespace ccl::codegen

#endif /* CCL_PROJECT_BASIC_CODEGEN_HPP */
