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

            constexpr static auto onMove(char /* chr */) noexcept -> void
            {
                // is not used
            }

            constexpr static auto onCharacter(char32_t /* chr */) noexcept -> void
            {
                // is not used
            }

            auto utfError(char /* chr */) noexcept -> void
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
        constexpr explicit CrtpBasicTextIterator(CrtpForkType /* unused */, T &from) noexcept
        {
            *this = std::bit_cast<CrtpBasicTextIterator>(from);
        }

        CCL_DECL auto fork() const noexcept -> ForkedTextIterator
        {
            return ForkedTextIterator{CrtpFork, *this};
        }

        CCL_DECL CCL_INLINE auto isInitialized() const noexcept -> bool
        {
            return initialized;
        }

        CCL_DECL CCL_INLINE auto getRemainingToFinishUtf() const noexcept -> u16
        {
            return remainingBytesToFinishSymbol;
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
            if (isInitialized()) {
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
            if (new_end < carriage) {
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

        constexpr auto skipCharacters(size_t n) noexcept(noexceptCarriageMove) -> void
        {
            for (auto i = as<size_t>(0); i != n; ++i) {
                next();
            }
        }

        constexpr auto moveToCleanChar() noexcept(noexceptCarriageMove) -> void
        {
            while (isLayout(futureChar(1))) {
                next();
            }
        }

        constexpr auto next() noexcept(noexceptCarriageMove) -> char32_t
        {
            moveCarriageToTheNextByte();

            while (remainingBytesToFinishSymbol != 0) {
                moveCarriageToTheNextByte();
            }

            return currentChar;
        }

        CCL_DECL auto futureChar(size_t times) const noexcept -> char32_t
        {
            auto fork = ForkedTextIterator{CrtpFork, *this};
            fork.skipCharacters(times);

            if (fork.errorDetected) {
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

        auto onUtfError(char chr) noexcept(noexceptCarriageMove) -> void
        {
            static_cast<CRTP &>(*this).utfError(chr);
        }

        constexpr auto moveCarriageToTheNextByte() noexcept(noexceptCarriageMove) -> char
        {
            if (!isInitialized()) {
                if (carriage == end) {
                    currentChar = 0;
                    return 0;
                }

                initialized = true;
            } else {
                if ((carriage + 1) >= end) {
                    carriage = end;
                    currentChar = 0;
                    return 0;
                }

                ++carriage;
            }

            modifyCurrentChar();
            return *carriage;
        }

        constexpr auto modifyCurrentChar() noexcept(noexceptCarriageMove) -> void
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

        constexpr auto trailingCharacterMove(char chr) noexcept(noexceptCarriageMove) -> void
        {
            if (!utf8::isTrailingCharacter(chr)) {
                onUtfError(chr);
            }

            currentChar <<= utf8::TrailingSize;
            currentChar |= as<char32_t>(as<std::byte>(chr) & ~utf8::ContinuationMask);
        }

        constexpr auto newCharacterMove(char chr) noexcept(noexceptCarriageMove) -> void
        {
            remainingBytesToFinishSymbol = utf8::size(chr);

            if (0 == remainingBytesToFinishSymbol) {
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

        [[noreturn]] static auto utfError(char /* chr */) -> void
        {
            throw std::logic_error{"unable to convert character to utf8"};
        }
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_BASIC_TEXT_ITERATOR_HPP */
