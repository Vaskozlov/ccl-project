#ifndef CERBERUS_PROJECT_LINE_TRACKER_HPP
#define CERBERUS_PROJECT_LINE_TRACKER_HPP

#include <cerberus/text/basic_text_iterator.hpp>

namespace cerb::text
{
    template<CharacterLiteral CharT>
    class LineTracker
    {
    public:
        CERBLIB_DECL auto get() const -> const BasicStringView<CharT> &
        {
            return line;
        }

        constexpr auto next() -> void
        {
            if (new_line_passed) {
                updateLine();
                new_line_passed = false;
            }

            if (basic_iterator.getCurrentChar() == '\n') {
                new_line_passed = true;
            }
        }

        constexpr explicit LineTracker(BasicTextIterator<CharT> &basic_iterator_)
          : basic_iterator{ basic_iterator_ }
        {
            updateLine();
        }

    private:
        constexpr auto updateLine() -> void
        {
            auto text = basic_iterator.getRemaining();
            auto line_begin = text.begin();
            auto length = text.find('\n');

            length = length == text.npos ? text.size() : length;
            line = { line_begin, length };
        }

        BasicStringView<CharT> line{};
        BasicTextIterator<CharT> &basic_iterator;
        bool new_line_passed{ false };
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_LINE_TRACKER_HPP */
