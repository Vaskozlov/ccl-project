#ifndef CCL_PROJECT_BASIC_TEXT_ITERATOR_HPP
#define CCL_PROJECT_BASIC_TEXT_ITERATOR_HPP

#include <ccl/char.hpp>
#include <isl/string_view.hpp>
#include <isl/utf8.hpp>

namespace ccl::text
{
    struct CCL_TRIVIAL_ABI CrtpForkType : private std::true_type
    {
    };

    constexpr inline auto CrtpFork = CrtpForkType{};

    template<typename CRTP>
    class CCL_TRIVIAL_ABI CrtpBasicTextIterator
    {
    public:
        using iterator = typename isl::string_view::iterator;

    private:
        iterator carriage{};
        iterator end{};
        char32_t currentChar{};
        u16 remainingBytesToFinishSymbol{};
        bool initialized{};

    public:
        class CCL_TRIVIAL_ABI ForkedTextIterator final
          : public CrtpBasicTextIterator<ForkedTextIterator>
        {
        public:
            using CrtpBasicTextIterator<ForkedTextIterator>::CrtpBasicTextIterator;

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

            CCL_INLINE auto utfError(char /* chr */) noexcept -> void
            {
                errorDetected = true;
            }

            bool errorDetected{false};
        };

        CrtpBasicTextIterator() noexcept = default;

        [[nodiscard]] constexpr explicit CrtpBasicTextIterator(isl::string_view input) noexcept
          : carriage{input.begin()}
          , end{input.end()}
        {}

        template<typename Source>
        [[nodiscard]] constexpr explicit CrtpBasicTextIterator(
            CrtpForkType /* unused */,
            Source &from) noexcept
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
            ISL_PREFETCH(carriage);
            const auto *it = getRemainingAsCarriage();

            if (it == end) [[unlikely]] {
                return {};
            }

            return *it;
        }

        CCL_DECL auto getRemaining() const noexcept -> isl::string_view
        {
            return {getRemainingAsCarriage(), end};
        }

        CCL_DECL auto getRemainingWithCurrent() const noexcept -> isl::string_view
        {
            return {carriage, end};
        }

        CCL_DECL auto isEOI() const noexcept -> bool
        {
            return getRemainingAsCarriage() == end;
        }

        CCL_DECL auto getRemainingText() const noexcept -> isl::string_view
        {
            if (isInitialized()) [[likely]] {
                return {carriage + isl::utf8::size(getNextCarriageValue()), end};
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

        constexpr auto skip(std::size_t n) CCL_NOEXCEPT_IF(moveCarriageToTheNextByte()) -> void
        {
            ISL_UNROLL_N(4)
            for (auto i = isl::as<std::size_t>(0); i != n; ++i) {
                moveCarriageToTheNextByte();
            }
        }

        constexpr auto moveToCleanChar() CCL_NOEXCEPT_IF(advance()) -> void
        {
            while (isLayout(futureChar())) {
                advance();
            }
        }

        constexpr auto advance() CCL_NOEXCEPT_IF(moveCarriageToTheNextByte()) -> char32_t
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
            fork.advance();

            if (fork.errorDetected) [[unlikely]] {
                return {};
            }

            return fork.getCurrentChar();
        }

    private:
        CCL_DECL auto toParent() noexcept CCL_LIFETIMEBOUND -> CRTP &
        {
            return static_cast<CRTP &>(*this);
        }

        CCL_DECL auto toParent() const noexcept CCL_LIFETIMEBOUND -> const CRTP &
        {
            return static_cast<const CRTP &>(*this);
        }

        constexpr auto onCarriageMove(char chr) CCL_NOEXCEPT_IF(toParent().onMove(chr)) -> void
        {
            toParent().onMove(chr);
        }

        constexpr auto onNextCharacter(char32_t chr)
            CCL_NOEXCEPT_IF(toParent().onCharacter(chr)) -> void
        {
            toParent().onCharacter(chr);
        }

        auto onUtfError(char chr) CCL_NOEXCEPT_IF(toParent().utfError(chr)) -> void
        {
            toParent().utfError(chr);
        }

        constexpr auto moveCarriageToTheNextByte() CCL_NOEXCEPT_IF(modifyCurrentChar()) -> char
        {
            if ((carriage + isl::as<std::size_t>(isInitialized())) >= end) [[unlikely]] {
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

        constexpr auto modifyCurrentChar() noexcept(
            noexcept(onNextCharacter(char32_t{})) && noexcept(onCarriageMove(char{})) &&
            noexcept(onUtfError(char{}))) -> void
        {
            using namespace std::string_view_literals;

            char chr = *carriage;
            onCarriageMove(chr);

            if (remainingBytesToFinishSymbol != 0) {
                trailingCharacterMove(chr);
            } else {
                newCharacterMove(chr);
            }

            if (0 == remainingBytesToFinishSymbol) {
                onNextCharacter(currentChar);
            }
        }

        constexpr auto trailingCharacterMove(char chr) CCL_NOEXCEPT_IF(onUtfError(chr)) -> void
        {
            if (!isl::utf8::isTrailingCharacter(chr)) [[unlikely]] {
                onUtfError(chr);
            }

            currentChar <<= isl::utf8::constants::TrailingSize;
            currentChar |= isl::as<char32_t>(
                isl::as<std::byte>(chr) & ~isl::utf8::constants::ContinuationMask);

            if (remainingBytesToFinishSymbol > 0) {
                --remainingBytesToFinishSymbol;
            }
        }

        constexpr auto newCharacterMove(char chr) CCL_NOEXCEPT_IF(onUtfError(chr)) -> void
        {
            remainingBytesToFinishSymbol = isl::utf8::size(chr);

            if (remainingBytesToFinishSymbol == 0) [[unlikely]] {
                onUtfError(chr);
            }

            currentChar = isl::as<char32_t>(
                isl::as<std::byte>(chr) & ~isl::utf8::getMask(remainingBytesToFinishSymbol));

            --remainingBytesToFinishSymbol;
        }
    };

    class CCL_TRIVIAL_ABI BasicTextIterator final : public CrtpBasicTextIterator<BasicTextIterator>
    {
    public:
        using CrtpBasicTextIterator<BasicTextIterator>::CrtpBasicTextIterator;

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

    extern template class CrtpBasicTextIterator<BasicTextIterator>;
}// namespace ccl::text

#endif /* CCL_PROJECT_BASIC_TEXT_ITERATOR_HPP */
