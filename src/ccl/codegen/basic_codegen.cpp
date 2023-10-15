#include "ccl/codegen/basic_codegen.hpp"
#include <stdexcept>

namespace ccl::codegen
{
    CCL_INLINE auto BasicCodeGenerator::getCurrentStream() noexcept -> std::string &
    {
        return generatedCode[streamId];
    }

    CCL_INLINE auto BasicCodeGenerator::getCurrentStream() const noexcept -> const std::string &
    {
        return generatedCode.at(streamId);
    }

    auto BasicCodeGenerator::getBackInserter() noexcept -> std::back_insert_iterator<std::string>
    {
        return std::back_inserter(getCurrentStream());
    }

    auto BasicCodeGenerator::newLine() -> void
    {
        getCurrentStream().push_back('\n');

        addScope(scopesCounter);
    }

    auto BasicCodeGenerator::addScope(size_t scopes_count) -> void
    {
        for (size_t i = 0; i < scopes_count * scopeSize; i += 4) {
            getCurrentStream().append("    ");
        }

        for (size_t i = 0; i != scopes_count * scopeSize % 4; ++i) {
            getCurrentStream().push_back(' ');
        }
    }

    auto BasicCodeGenerator::getCode() const noexcept -> std::string
    {
        auto result = std::string{};

        for (const auto &[id, stream] : generatedCode) {
            result.append(stream);
        }

        return result;
    }

    auto BasicCodeGenerator::reserve(size_t size) -> void
    {
        getCurrentStream().reserve(size);
    }

    auto BasicCodeGenerator::operator<<(ScopeSize scope_size) -> BasicCodeGenerator &
    {
        if (0 == scope_size.size) [[unlikely]] {
            throw std::logic_error{"Scope size can not be equal to zero."};
        }

        scopeSize = scope_size.size;
        return *this;
    }

    auto BasicCodeGenerator::operator<<(PushScope /* unused */) -> BasicCodeGenerator &
    {
        ++scopesCounter;
        addScope(1);
        return *this;
    }

    auto BasicCodeGenerator::operator<<(PopScope /* unused */) -> BasicCodeGenerator &
    {
        if (scopesCounter == 0) [[unlikely]] {
            throw std::runtime_error{"Cannot pop scope, because there are not any of them"};
        }

        --scopesCounter;
        return *this;
    }

    auto BasicCodeGenerator::operator<<(StreamId stream_id) -> BasicCodeGenerator &
    {
        streamId = stream_id.streamId;
        return *this;
    }

    auto BasicCodeGenerator::operator<<(char character) -> BasicCodeGenerator &
    {
        if (character == '\n') [[unlikely]] {
            newLine();
        } else [[likely]] {
            getCurrentStream().push_back(character);
        }

        return *this;
    }

    auto BasicCodeGenerator::operator<<(isl::string_view string) -> BasicCodeGenerator &
    {
        return *this << as<std::string_view>(string);
    }

    auto BasicCodeGenerator::operator<<(std::string_view string) -> BasicCodeGenerator &
    {
        getCurrentStream().reserve(generatedCode.size() + string.size());

        for (char character : string) {
            *this << character;
        }

        return *this;
    }

    auto BasicCodeGenerator::operator<<(const std::string &string) -> BasicCodeGenerator &
    {
        return *this << isl::as<std::string_view>(string);
    }
}// namespace ccl::codegen
