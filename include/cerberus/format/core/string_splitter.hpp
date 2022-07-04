#ifndef CERBERUS_PROJECT_STRING_SPLITTER_HPP
#define CERBERUS_PROJECT_STRING_SPLITTER_HPP

#include <array>
#include <cerberus/format/core/arguments_counter.hpp>
#include <cerberus/format/core/block_filler.hpp>

namespace cerb::fmt
{
    template<TemplateString String>
    struct StringSplitter
    {
    public:
        using CharT = decltype(String.zeroChar());
        using value_type = std::basic_string_view<CharT>;
        static constexpr size_t blocks_num = core::countArgs(String.strView()) + 1;

        CERBLIB_DECL auto getBlocks() const -> const std::array<value_type, blocks_num> &
        {
            return blocks;
        }

        StringSplitter() = default;

    private:
        static constexpr std::array<value_type, blocks_num> blocks =
            core::fillBlocks<blocks_num, CharT>(String.strView());
    };
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_STRING_SPLITTER_HPP */
