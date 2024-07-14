#ifndef CCL_PROJECT_BASIC_CODEGEN_HPP
#define CCL_PROJECT_BASIC_CODEGEN_HPP

#include <ccl/ccl.hpp>
#include <isl/const_string.hpp>
#include <isl/string_view.hpp>
#include <map>

namespace ccl::codegen
{
    class BasicCodeGenerator
    {
    public:
        struct CCL_TRIVIAL_ABI PushScope : private std::true_type
        {
        };

        struct CCL_TRIVIAL_ABI PopScope : private std::false_type
        {
        };

        struct CCL_TRIVIAL_ABI Endl : private std::true_type
        {
        };

        struct CCL_TRIVIAL_ABI ScopeSize
        {
            std::size_t size = 4;
        };

        struct CCL_TRIVIAL_ABI StreamId
        {
            Id streamId = 0;
        };

    private:
        isl::Map<Id, std::string> generatedCode;
        Id streamId{};
        std::size_t scopesCounter{};
        std::size_t scopeSize{4};

    public:
        BasicCodeGenerator()
        {
            generatedCode.try_emplace(streamId);
        };

        [[nodiscard]] auto getCode() const noexcept -> std::string;

        auto reserve(std::size_t size) -> void;

        auto operator<<(ScopeSize scope_size) CCL_LIFETIMEBOUND->BasicCodeGenerator &;
        auto operator<<(PushScope /* unused */) CCL_LIFETIMEBOUND->BasicCodeGenerator &;
        auto operator<<(PopScope /* unused */) CCL_LIFETIMEBOUND->BasicCodeGenerator &;
        auto operator<<(StreamId stream_id) CCL_LIFETIMEBOUND->BasicCodeGenerator &;

        auto operator<<(char character) CCL_LIFETIMEBOUND->BasicCodeGenerator &;

        auto operator<<(const isl::RangeOf<char> auto &range_of_chars)
            CCL_LIFETIMEBOUND->BasicCodeGenerator &
        {
            for (auto chr : range_of_chars) {
                *this << chr;
            }

            return *this;
        }

        template<isl::AnyTrait<std::is_integral, std::is_floating_point> Numeric>
        auto operator<<(Numeric value) CCL_LIFETIMEBOUND->BasicCodeGenerator &
        {
            return *this << std::string_view{fmt::to_string(value)};
        }

        auto operator<<(Endl /* unused */) CCL_LIFETIMEBOUND->BasicCodeGenerator &
        {
            newLine();
            return *this;
        }

        template<std::size_t N>// NOLINTNEXTLINE
        auto operator<<(const isl::CArray<char, N> &character_array)
            CCL_LIFETIMEBOUND->BasicCodeGenerator &
        {
            return *this << isl::string_view{character_array};// NOLINT
        }

    private:
        [[nodiscard]] auto getCurrentStream() noexcept CCL_LIFETIMEBOUND -> std::string &;

        auto newLine() -> void;
        auto addScope(std::size_t scopes_count) -> void;
    };

    constexpr auto setScopeSize(std::size_t size) noexcept -> BasicCodeGenerator::ScopeSize
    {
        return {.size = size};
    }

    constexpr auto setStream(Id stream_id) noexcept -> BasicCodeGenerator::StreamId
    {
        return {.streamId = stream_id};
    }

    constexpr inline auto endl = BasicCodeGenerator::Endl{};
    constexpr inline auto push_scope = BasicCodeGenerator::PushScope{};
    constexpr inline auto pop_scope = BasicCodeGenerator::PopScope{};

}// namespace ccl::codegen

#endif /* CCL_PROJECT_BASIC_CODEGEN_HPP */
