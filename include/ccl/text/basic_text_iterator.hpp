#ifndef CCL_PROJECT_BASIC_TEXT_ITERATOR_HPP
#define CCL_PROJECT_BASIC_TEXT_ITERATOR_HPP

#include <ccl/char.hpp>
#include <ccl/string_view.hpp>
#include <ccl/utf8.hpp>

namespace ccl::text
{
    struct CCL_TRIVIAL_ABI CrtpForkType : public std::true_type
    {
    };

    constexpr auto CrtpFork = CrtpForkType{};

    template<typename CRTP>
    class CCL_TRIVIAL_ABI CrtpBasicTextIterator
    {
    public:
        using iterator = typename string_view::iterator;

        class CCL_TRIVIAL_ABI ForkedTextIterator final
          : public CrtpBasicTextIterator<ForkedTextIterator>
        {
        public:
            constexpr explicit ForkedTextIterator(string_view input) noexcept
              : CrtpBasicTextIterator<ForkedTextIterator>(input)
            {}

            template<typename T>
            constexpr explicit ForkedTextIterator(CrtpForkType /* unused */, T &other) noexcept
              : CrtpBasicTextIterator<ForkedTextIterator>(CrtpFork, other)
            {}

            CCL_INLINE constexpr static auto onMove(char /* chr */) noexcept -> void
            {
                // is not used
            }

            CCL_INLINE constexpr static auto onCharacter(char32_t /* chr */) noexcept -> void
            {
                // is not used
            }

            CCL_INLINE auto utfError(char /* chr */) noexcept -> void
            {
                errorDetected = true;
            }

            bool errorDetected{false};
        };

        constexpr static auto noexceptCarriageMove = std::is_same_v<CRTP, ForkedTextIterator>;

        CrtpBasicTextIterator() noexcept = default;

        CCL_INLINE constexpr explicit CrtpBasicTextIterator(string_view input) noexcept
          : carriage{input.begin()}
          , end{input.end()}
        {}

        template<typename T>
        CCL_INLINE constexpr explicit CrtpBasicTextIterator(
            CrtpForkType /* unused */, T &from) noexcept
          : carriage{from.getCarriage()}
          , end{from.getEnd()}
          , currentChar{from.getCurrentChar()}
          , remainingBytesToFinishSymbol{from.getRemainingBytesToFinishSymbol()}
          , initialized{from.isInitialized()}
        {}

        CCL_DECL auto fork() const noexcept -> ForkedTextIterator
        {
            return ForkedTextIterator{CrtpFork, *this};
        }

        CCL_DECL auto isInitialized() const noexcept -> bool
        {
            return initialized;
        }

        CCL_DECL auto getCarriage() const noexcept -> iterator
        {
            return carriage;
        }

        CCL_DECL auto getEnd() const noexcept -> iterator
        {
            return end;
        }

        CCL_DECL auto getRemainingBytesToFinishSymbol() const noexcept -> u16
        {
            return remainingBytesToFinishSymbol;
        }

        CCL_DECL auto getRemainingAsCarriage() const noexcept -> iterator
        {
            if (!isInitialized()) [[unlikely]] {
                return carriage;
            }

            return std::min(carriage + 1, end);
        }

        CCL_DECL auto getNextCarriageValue() const noexcept -> char
        {
            CCL_PREFETCH(carriage);
            const auto *it = getRemainingAsCarriage();

            if (it == end) [[unlikely]] {
                return {};
            }

            return *it;
        }

        CCL_DECL auto getRemaining() const noexcept -> string_view
        {
            return {getRemainingAsCarriage(), end};
        }

        CCL_DECL auto getRemainingWithCurrent() const noexcept -> string_view
        {
            return {carriage, end};
        }

        CCL_DECL auto isEOI() const noexcept -> bool
        {
            return getRemainingAsCarriage() == end;
        }

        CCL_DECL auto getFutureRemaining() const noexcept -> string_view
        {
            if (isInitialized()) [[likely]] {
                return {carriage + utf8::size(getNextCarriageValue()), end};
            }

            return {carriage, end};
        }

        CCL_DECL auto getCurrentChar() const noexcept -> char32_t
        {
            return currentChar;
        }

        constexpr auto setCurrentChar(char32_t new_current_char) noexcept -> void
        {
            currentChar = new_current_char;
        }

        constexpr auto setEnd(iterator new_end) -> void
        {
            if (new_end < carriage) [[unlikely]] {
                throw std::invalid_argument{"end must be above carriage"};
            }

            end = new_end;
        }

        constexpr auto skip(size_t n) noexcept(noexceptCarriageMove) -> void
        {
            CCL_UNROLL_N(4)
            for (auto i = as<size_t>(0); i != n; ++i) {
                moveCarriageToTheNextByte();
            }
        }

        constexpr auto moveToCleanChar() noexcept(noexceptCarriageMove) -> void
        {
            while (isLayout(futureChar())) {
                next();
            }
        }

        constexpr auto next() noexcept(noexceptCarriageMove) -> char32_t
        {
            moveCarriageToTheNextByte();

            while (remainingBytesToFinishSymbol != 0) [[unlikely]] {
                moveCarriageToTheNextByte();
            }

            return currentChar;
        }

        CCL_DECL auto futureChar() const noexcept -> char32_t
        {
            auto fork = ForkedTextIterator{CrtpFork, *this};
            fork.next();

            if (fork.errorDetected) [[unlikely]] {
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

        CCL_INLINE auto onUtfError(char chr) noexcept(noexceptCarriageMove) -> void
        {
            static_cast<CRTP &>(*this).utfError(chr);
        }

        constexpr auto moveCarriageToTheNextByte() noexcept(noexceptCarriageMove) -> char
        {
            CCL_PREFETCH(carriage);

            if ((carriage + 1) >= end) [[unlikely]] {
                carriage = end;
                currentChar = 0;
                return 0;
            }

            if (!isInitialized()) [[unlikely]] {
                initialized = true;
            } else [[likely]] {
                ++carriage;
            }

            modifyCurrentChar();
            return *carriage;
        }

        CCL_INLINE constexpr auto modifyCurrentChar() noexcept(noexceptCarriageMove) -> void
        {
            using namespace std::string_view_literals;

            auto chr = *carriage;
            onCarriageMove(chr);

            if (remainingBytesToFinishSymbol != 0) {
                trailingCharacterMove(chr);
            } else {
                newCharacterMove(chr);
            }

            --remainingBytesToFinishSymbol;

            if (0 == remainingBytesToFinishSymbol) {
                onNextCharacter(currentChar);
            }
        }

        CCL_INLINE constexpr auto trailingCharacterMove(char chr) noexcept(noexceptCarriageMove)
            -> void
        {
            if (!utf8::isTrailingCharacter(chr)) [[unlikely]] {
                onUtfError(chr);
            }

            currentChar <<= utf8::TrailingSize;
            currentChar |= as<char32_t>(as<std::byte>(chr) & ~utf8::ContinuationMask);
        }

        constexpr auto newCharacterMove(char chr) noexcept(noexceptCarriageMove) -> void
        {
            remainingBytesToFinishSymbol = utf8::size(chr);

            if (0 == remainingBytesToFinishSymbol) [[unlikely]] {
                onUtfError(chr);
            }

            currentChar =
                as<char32_t>(as<std::byte>(chr) & ~utf8::getMask(remainingBytesToFinishSymbol));
        }

        iterator carriage{};
        iterator end{};
        char32_t currentChar{};
        u16 remainingBytesToFinishSymbol{};
        bool initialized{};
    };

    class CCL_TRIVIAL_ABI BasicTextIterator final : public CrtpBasicTextIterator<BasicTextIterator>
    {
    public:
        BasicTextIterator() noexcept = default;

        constexpr explicit BasicTextIterator(string_view input) noexcept
          : CrtpBasicTextIterator<BasicTextIterator>(input)
        {}

        constexpr static auto onMove(char /* chr */) noexcept -> void
        {
            // no action by the default
        }

        constexpr static auto onCharacter(char32_t /* chr */) noexcept -> void
        {
            // no action by the default
        }

        [[noreturn]] CCL_INLINE static auto utfError(char /* chr */) -> void
        {
            throw std::logic_error{"unable to convert character to utf8"};
        }
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_BASIC_TEXT_ITERATOR_HPP */
