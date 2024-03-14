module;

#include <ccl/defines.hpp>

export module ccl.codegen;
export import ccl.core;
export import isl;

export namespace ccl::codegen
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
            std::size_t size = 4;
        };

        struct CCL_TRIVIAL_ABI StreamId
        {
            Id streamId = 0;
        };

    private:
        std::map<Id, std::string> generatedCode{};
        Id streamId = 0;
        std::size_t scopesCounter = 0;
        std::size_t scopeSize = 4;

    public:
        BasicCodeGenerator() = default;

        [[nodiscard]] auto getCode() const noexcept -> std::string;

        [[nodiscard]] auto
            getBackInserter() noexcept CCL_LIFETIMEBOUND -> std::back_insert_iterator<std::string>;

        auto reserve(std::size_t size) -> void;

        auto operator<<(ScopeSize scope_size) CCL_LIFETIMEBOUND->BasicCodeGenerator &;
        auto operator<<(PushScope /* unused */) CCL_LIFETIMEBOUND->BasicCodeGenerator &;
        auto operator<<(PopScope /* unused */) CCL_LIFETIMEBOUND->BasicCodeGenerator &;
        auto operator<<(StreamId stream_id) CCL_LIFETIMEBOUND->BasicCodeGenerator &;

        auto operator<<(char character) CCL_LIFETIMEBOUND->BasicCodeGenerator &;
        auto operator<<(isl::string_view string) CCL_LIFETIMEBOUND->BasicCodeGenerator &;
        auto operator<<(std::string_view string) CCL_LIFETIMEBOUND->BasicCodeGenerator &;
        auto operator<<(const std::string &string) CCL_LIFETIMEBOUND->BasicCodeGenerator &;

        template<typename Numeric>
        auto operator<<(Numeric value) CCL_LIFETIMEBOUND->BasicCodeGenerator &
            requires std::floating_point<Numeric> || std::integral<Numeric>
        {
            return *this << std::string_view{std::to_string(value)};
        }

        auto operator<<(Endl /* unused */) CCL_LIFETIMEBOUND->BasicCodeGenerator &
        {
            newLine();
            return *this;
        }

        template<std::size_t Size>
        auto
            operator<<(const isl::ConstString<Size> &string) CCL_LIFETIMEBOUND->BasicCodeGenerator &
        {
            return *this << as<isl::string_view>(string);
        }

        template<std::size_t N>// NOLINTNEXTLINE
        auto operator<<(const isl::CArray<char, N> &character_array)
            CCL_LIFETIMEBOUND->BasicCodeGenerator &
        {
            return *this << std::string_view{character_array};// NOLINT
        }

    private:
        [[nodiscard]] auto getCurrentStream() noexcept CCL_LIFETIMEBOUND -> std::string &;
        [[nodiscard]] auto
            getCurrentStream() const noexcept CCL_LIFETIMEBOUND -> const std::string &;

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
