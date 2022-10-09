#ifndef CCL_PROJECT_BASIC_TEXT_ITERATOR_HPP
#define CCL_PROJECT_BASIC_TEXT_ITERATOR_HPP

#include <ccl/char.hpp>
#include <ccl/string_view.hpp>
#include <ccl/utf8.hpp>

namespace ccl::text
{
    struct CCL_TRIVIAL_ABI CrtpForkType : std::true_type
    {
    };

    constexpr auto CrtpFork = CrtpForkType{};

    template<typename CRTP>
    class CCL_TRIVIAL_ABI CrtpBasicTextIterator
    {
    public:
        using iterator = typename string_view::iterator;

        struct CCL_TRIVIAL_ABI ForkedTextIterator final
          : public CrtpBasicTextIterator<ForkedTextIterator>
        {
            constexpr explicit ForkedTextIterator(string_view text_) noexcept
              : CrtpBasicTextIterator<ForkedTextIterator>(text_)
            {}

            template<typename T>
            constexpr explicit ForkedTextIterator(CrtpForkType /* unused */, T &other) noexcept
              : CrtpBasicTextIterator<ForkedTextIterator>(CrtpFork, other)
            {}

            constexpr static auto onMove(char /* chr */) noexcept -> void
            {}

            constexpr static auto onCharacter(char32_t /* chr */) noexcept -> void
            {}

            auto utfError(char /* chr */) noexcept -> void
            {
                error_detected = true;
            }

            bool error_detected{ false };
        };

        static constexpr auto noexcept_carriage_move = std::is_same_v<CRTP, ForkedTextIterator>;

        CrtpBasicTextIterator() noexcept = default;

        CCL_INLINE constexpr explicit CrtpBasicTextIterator(string_view text_) noexcept
          : carriage{ text_.begin() }, end{ text_.end() }
        {}

        template<typename T>
        constexpr explicit CrtpBasicTextIterator(CrtpForkType /* unused */, T &from) noexcept
        {
            *this = std::bit_cast<CrtpBasicTextIterator>(from);
        }

        CCL_DECL auto fork() const noexcept -> ForkedTextIterator
        {
            return ForkedTextIterator{ CrtpFork, *this };
        }

        CCL_DECL CCL_INLINE auto isInitialized() const noexcept -> bool
        {
            return initialized;
        }

        CCL_DECL CCL_INLINE auto getRemainingToFinishUtf() const noexcept -> u16
        {
            return remaining_to_finish_utf;
        }

        CCL_DECL CCL_INLINE auto getCarriage() const noexcept -> iterator
        {
            return carriage;
        }

        CCL_DECL CCL_INLINE auto getEnd() const noexcept -> iterator
        {
            return end;
        }

        CCL_DECL CCL_INLINE auto getRemainingAsCarriage() const noexcept -> iterator
        {
            if (isInitialized()) {
                return std::min(carriage + 1, end);
            }

            return carriage;
        }

        CCL_DECL auto getNextCarriageValue() const noexcept -> char
        {
            const auto *it = getRemainingAsCarriage();

            if (it == end) {
                return {};
            }

            return *it;
        }

        CCL_DECL auto getRemaining() const noexcept -> string_view
        {
            return { getRemainingAsCarriage(), end };
        }

        CCL_DECL auto getRemainingWithCurrent() const noexcept -> string_view
        {
            return { carriage, end };
        }

        CCL_DECL auto isEOI() const noexcept -> bool
        {
            return getRemainingAsCarriage() == end;
        }

        template<typename T = string_view>
        CCL_DECL auto getFutureRemaining() const noexcept -> T
        {
            auto carriage_move = isInitialized() ? utf8::utfSize(getNextCarriageValue()) : 0;
            return { carriage + carriage_move, end };
        }

        CCL_DECL auto getCurrentChar() const noexcept -> char32_t
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
                throw std::invalid_argument{ "end must be above carriage" };
            }

            end = new_end;
        }

        constexpr auto skip(size_t n) noexcept(noexcept_carriage_move) -> void
        {
            CCL_UNROLL_N(4)
            for (auto i = 0ZU; i != n; ++i) {
                moveCarriage();
            }
        }

        constexpr auto skipCharacters(size_t n) noexcept(noexcept_carriage_move) -> void
        {
            for (auto i = 0ZU; i != n; ++i) {
                next();
            }
        }

        constexpr auto moveToCleanChar() noexcept(noexcept_carriage_move) -> void
        {
            while (isLayout(futureChar(1))) {
                next();
            }
        }

        constexpr auto next() noexcept(noexcept_carriage_move) -> char32_t
        {
            do {
                moveCarriage();
            } while (remaining_to_finish_utf != 0);

            return current_char;
        }

        CCL_DECL auto futureChar(size_t times) const noexcept -> char32_t
        {
            auto fork = ForkedTextIterator{ CrtpFork, *this };
            fork.skipCharacters(times);

            if (fork.error_detected) {
                return {};
            }

            return fork.getCurrentChar();
        }

    private:
        constexpr auto onCarriageMove(char chr) -> void
        {
            static_cast<CRTP &>(*this).onMove(chr);
        }

        constexpr auto onNextCharacter(char32_t chr) -> void
        {
            static_cast<CRTP &>(*this).onCharacter(chr);
        }

        auto onUtfError(char chr) noexcept(noexcept_carriage_move) -> void
        {
            static_cast<CRTP &>(*this).utfError(chr);
        }

        constexpr auto moveCarriage() noexcept(noexcept_carriage_move) -> char
        {
            if (not isInitialized()) {
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

        constexpr auto modifyCurrentChar() noexcept(noexcept_carriage_move) -> void
        {
            using namespace std::string_view_literals;

            auto chr = *carriage;
            onCarriageMove(chr);

            if (remaining_to_finish_utf != 0) {
                trailingCharacterMove(chr);
            } else {
                newCharacterMove(chr);
            }

            --remaining_to_finish_utf;

            if (remaining_to_finish_utf == 0) {
                onNextCharacter(current_char);
            }
        }

        constexpr auto trailingCharacterMove(char chr) noexcept(noexcept_carriage_move) -> void
        {
            if (not utf8::isTrailingCharacter(chr)) {
                onUtfError(chr);
            }

            current_char <<= utf8::TrailingSize;
            current_char |= chr & static_cast<char>(~utf8::ContinuationMask);
        }

        constexpr auto newCharacterMove(char chr) noexcept(noexcept_carriage_move) -> void
        {
            remaining_to_finish_utf = utf8::utfSize(chr);

            if (remaining_to_finish_utf == 0) {
                onUtfError(chr);
            }

            current_char = std::bit_cast<unsigned>(
                chr & static_cast<char>(~utf8::getMask(remaining_to_finish_utf)));
        }

        iterator carriage{};
        iterator end{};
        char32_t current_char{};
        u16 remaining_to_finish_utf{};
        bool initialized{};
    };

    struct CCL_TRIVIAL_ABI BasicTextIterator final : public CrtpBasicTextIterator<BasicTextIterator>
    {
        using Base = CrtpBasicTextIterator<BasicTextIterator>;

        BasicTextIterator() noexcept = default;

        constexpr explicit BasicTextIterator(string_view text_) noexcept
          : CrtpBasicTextIterator<BasicTextIterator>(text_)
        {}

        constexpr static auto onMove(char /* chr */) noexcept -> void
        {}

        constexpr static auto onCharacter(char32_t /* chr */) noexcept -> void
        {}

        static auto utfError(char /* chr */) -> void
        {
            throw std::logic_error{ "unable to convert character to utf8" };
        }
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_BASIC_TEXT_ITERATOR_HPP */
