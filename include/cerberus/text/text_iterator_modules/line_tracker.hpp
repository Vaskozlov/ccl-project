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

        constexpr auto next(BasicTextIterator<CharT> &basic_iterator_) -> void
        {
            basic_iterator = &basic_iterator_;

            if (new_line_passed) {
                updateLine();
                new_line_passed = false;
            }

            if (basic_iterator->getCurrentChar() == '\n') {
                new_line_passed = true;
            }
        }

        constexpr explicit LineTracker(BasicTextIterator<CharT> &basic_iterator_)
          : basic_iterator(&basic_iterator_)
        {
            updateLine();
        }

    private:
        constexpr auto updateLine() -> void
        {
            auto text = basic_iterator->getRemaining();
            auto line_begin = text.begin();
            auto length = text.find('\n');

            length = length == text.npos ? text.size() : length;
            line = { line_begin, length };
        }

        StrView<CharT> line{};
        BasicTextIterator<CharT> *basic_iterator{ nullptr };
        bool new_line_passed{ false };
    };
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_LINE_TRACKER_HPP */
