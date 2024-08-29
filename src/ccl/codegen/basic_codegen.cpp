#include "ccl/codegen/basic_codegen.hpp"
#include <stdexcept>

namespace ccl::codegen
{
    CCL_INLINE auto BasicCodeGenerator::getCurrentStream() noexcept -> std::string &
    {
        return generatedCode[streamId];
    }

    auto BasicCodeGenerator::newLine() -> void
    {
        getCurrentStream().push_back('\n');
        addScope(scopesCounter);
    }

    auto BasicCodeGenerator::addScope(std::size_t scopes_count) -> void
    {
        const auto whitespaces_count = scopes_count * scopeSize;

        for (std::size_t i = 0; i < whitespaces_count; i += 4) {
            getCurrentStream().append("    ");
        }

        for (std::size_t i = 0; i != whitespaces_count % 4; ++i) {
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

    auto BasicCodeGenerator::reserve(std::size_t size) -> void
    {
        getCurrentStream().reserve(size);
    }

    auto BasicCodeGenerator::operator<<(ScopeSize scope_size) -> BasicCodeGenerator &
    {
        if (scope_size.size == 0) [[unlikely]] {
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
            throw std::logic_error{"Cannot pop scope, because there are not any of them"};
        }

        --scopesCounter;
        return *this;
    }

    auto BasicCodeGenerator::operator<<(StreamId stream_id) -> BasicCodeGenerator &
    {
        streamId = stream_id.streamId;
        generatedCode.try_emplace(streamId);
        return *this;
    }

    auto BasicCodeGenerator::operator<<(char character) -> BasicCodeGenerator &
    {
        if (character == '\n') {
            newLine();
            return *this;
        }

        getCurrentStream().push_back(character);
        return *this;
    }
}// namespace ccl::codegen
