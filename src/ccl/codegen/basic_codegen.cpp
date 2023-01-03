#include "ccl/codegen/basic_codegen.hpp"
#include "ccl/flatmap.hpp"
#include <stdexcept>

namespace ccl::codegen
{
    constexpr bool HasStdSplit =
        requires(std::string_view string) { std::views::split(string, '\n'); };

    auto BasicCodeGenerator::newLine() -> void
    {
        generatedCode.push_back('\n');

        addScope(scopesCounter);
    }

    auto BasicCodeGenerator::addScope(size_t scopes_count) -> void
    {
        for (size_t i = 0; i < scopes_count * scopeSize; i += 4) {
            generatedCode.append("    ");
        }

        for (size_t i = 0; i != scopes_count * scopeSize % 4; ++i) {
            generatedCode.push_back(' ');
        }
    }

    auto BasicCodeGenerator::operator<<(ScopeSize scope_size) -> BasicCodeGenerator &
    {
        if (0 == scope_size.size) {
            throw std::logic_error("Scope size can not be equal to zero.");
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
        if (scopesCounter == 0) {
            throw std::runtime_error("Cannot pop scope, because there are not any of them");
        }

        --scopesCounter;
        return *this;
    }

    auto BasicCodeGenerator::operator<<(char character) -> BasicCodeGenerator &
    {
        if (character == '\n') {
            newLine();
        } else {
            generatedCode.push_back(character);
        }

        return *this;
    }

    auto BasicCodeGenerator::operator<<(string_view string) -> BasicCodeGenerator &
    {
        return *this << as<std::string_view>(string);
    }

    auto BasicCodeGenerator::operator<<(std::string_view string) -> BasicCodeGenerator &
    {
        generatedCode.reserve(generatedCode.size() + string.size());

        for (auto character : string) {
            *this << character;
        }

        return *this;
    }

    auto BasicCodeGenerator::operator<<(const std::string &string) -> BasicCodeGenerator &
    {
        return *this << as<std::string_view>(string);
    }
}// namespace ccl::codegen
