#ifndef CERBERUS_PROJECT_BASIC_TEXT_ITERATOR_HPP
#define CERBERUS_PROJECT_BASIC_TEXT_ITERATOR_HPP

#include <cerberus/char.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/utf8.hpp>

namespace cerb::text
{
    class BasicTextIterator
    {
    private:
        using iterator = typename u8string_view ::iterator;

    public:
        CERBLIB_DECL auto getCarriage() const noexcept -> iterator
        {
            return carriage;
        }

        CERBLIB_DECL auto getEnd() const noexcept -> iterator
        {
            return end;
        }

        CERBLIB_DECL auto getRemainingAsCarriage() const noexcept -> iterator
        {
            if (initialized) {
                return std::min(carriage + 1, end);
            }

            return carriage;
        }

        CERBLIB_DECL auto getRemaining() const noexcept -> u8string_view
        {
            return { getRemainingAsCarriage(), end };
        }

        CERBLIB_DECL auto getRemainingWithCurrent() const noexcept -> u8string_view
        {
            return { carriage, end };
        }

        CERBLIB_DECL auto isEnd() const -> bool
        {
            return getRemainingAsCarriage() == end;
        }

        CERBLIB_DECL auto getFutureRemaining(size_t times) const -> u8string_view
        {
            auto fork = *this;
            fork.rawSymbolsSkip(times);
            return fork.getRemainingWithCurrent();
        }

        CERBLIB_DECL auto getCurrentChar() const noexcept -> char32_t
        {
            return current_char;
        }

        constexpr auto setCurrentChar(char32_t new_current_char) noexcept -> void
        {
            current_char = new_current_char;
        }

        constexpr auto setEnd(iterator new_end) -> void
        {
            if (new_end < carriage) {
                throw InvalidArgument{ "end must be above carriage" };
            }

            end = new_end;
        }

        constexpr auto rawSkip(size_t n) -> void
        {
            for (size_t i = 0; i != n; ++i) {
                moveCarriage();
            }
        }

        constexpr auto rawSymbolsSkip(size_t n) -> void
        {
            for (size_t i = 0; i != n; ++i) {
                nextRawChar();
            }
        }

        constexpr auto moveToCleanChar() -> void
        {
            while (isLayout(futureRawChar(1))) {
                nextRawChar();
            }
        }

        constexpr auto nextRawChar() -> char32_t
        {
            do {
                moveCarriage();
            } while (remaining_to_finish_utf != 0);

            return current_char;
        }

        CERBLIB_DECL auto futureRawChar(size_t times) const -> char32_t
        {
            auto forked = *this;

            for (size_t i = 0; i != times; ++i) {
                forked.nextRawChar();
            }

            return forked.getCurrentChar();
        }

        constexpr virtual auto onMove(char8_t /* chr */) -> void
        {}

        constexpr virtual auto onCharacter(char32_t /* chr */) -> void
        {}

        virtual auto onUtfError(char8_t /* chr */) -> void
        {
            using namespace std::string_view_literals;

            throw utf8::Utf8ConvertionError{ "unable to convert character to utf8"sv };
        }

        auto operator=(const BasicTextIterator &) -> BasicTextIterator & = default;
        auto operator=(BasicTextIterator &&) noexcept -> BasicTextIterator & = default;

        BasicTextIterator() = default;
        BasicTextIterator(const BasicTextIterator &) = default;
        BasicTextIterator(BasicTextIterator &&) noexcept = default;

        constexpr explicit BasicTextIterator(u8string_view text_)
          : carriage{ text_.begin() }, end{ text_.end() }
        {}

        constexpr virtual ~BasicTextIterator(){};// NOLINT gcc says, that destructor is used before
                                                 // its definition

    private:
        constexpr auto moveCarriage() -> char8_t
        {
            if (not initialized) {
                if (carriage == end) {
                    current_char = 0;
                    return 0;
                }

                initialized = true;
            } else {
                if (carriage + 1 >= end) {
                    carriage = end;
                    current_char = 0;
                    return 0;
                }

                ++carriage;
            }

            modifyCurrentChar();
            return *carriage;
        }

        constexpr auto modifyCurrentChar() -> void
        {
            using namespace std::string_view_literals;

            auto chr = *carriage;
            onMove(chr);

            if (remaining_to_finish_utf != 0) {
                if (not utf8::isTrailingCharacter(chr)) {
                    onUtfError(chr);
                }

                current_char <<= utf8::TrailingSize;
                current_char |= chr & static_cast<char8_t>(~utf8::ContinuationMask);
            } else {
                remaining_to_finish_utf = utf8::utfSize(chr);

                if (remaining_to_finish_utf == 0) {
                    onUtfError(chr);
                }

                current_char = chr & static_cast<char8_t>(~utf8::getMask(remaining_to_finish_utf));
            }

            --remaining_to_finish_utf;

            if (remaining_to_finish_utf == 0) {
                onCharacter(current_char);
            }
        }

        constexpr auto skipBadUtf() -> void
        {
            auto fork = *this;

            while (true) {
                auto chr = fork.moveCarriage();

                if (utf8::utfSize(chr) != 0) {
                    break;
                }

                moveCarriage();
            }
        }

        iterator carriage{};
        iterator end{};
        char32_t current_char{};
        u16 remaining_to_finish_utf{};
        bool initialized{};
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_BASIC_TEXT_ITERATOR_HPP */
