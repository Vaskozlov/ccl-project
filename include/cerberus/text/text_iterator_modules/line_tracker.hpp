#ifndef CERBERUS_PROJECT_LINE_TRACKER_HPP
#define CERBERUS_PROJECT_LINE_TRACKER_HPP

#include <cerberus/text/basic_text_iterator.hpp>

namespace cerb::text::module
{
    template<CharacterLiteral CharT>
    class LineTracker
    {
    public:
        CERBLIB_DECL auto get() const -> const StrView<CharT> &
        {
            return line;
        }

        constexpr auto next(CharT chr) -> void
        {
            if (new_line_passed) {
                updateLine();
                new_line_passed = false;
            }

            if (chr == '\n') {
                new_line_passed = true;
            }
        }

        constexpr explicit LineTracker(StrView<CharT> text_)
          : text{ text_ }, line{ text.begin(), std::min(text.size(), text.find('\n')) }
        {}

    private:
        constexpr auto updateLine() -> void
        {
            auto new_line_begin = std::min(text.end(), line.end() + 1);
            auto end_offset = std::min(text.size(), text.find('\n', new_line_begin));

            line = { new_line_begin, text.begin() + end_offset };
        }

        StrView<CharT> text{};
        StrView<CharT> line{};
        bool new_line_passed{ false };
    };
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_LINE_TRACKER_HPP */
