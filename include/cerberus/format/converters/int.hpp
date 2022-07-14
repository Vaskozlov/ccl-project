#ifndef CERBERUS_PROJECT_INT_HPP
#define CERBERUS_PROJECT_INT_HPP

#include <cerberus/char.hpp>
#include <cerberus/format/core/exceptions.hpp>
#include <fmt/format.h>
#include <string>

namespace cerb::fmt
{
    enum Notation : u16
    {
        BINARY = 2,
        OCTAL = 8,
        DECIMAL = 10,
        HEXADECIMAL = 16
    };

    CERBLIB_EXCEPTION(IntegralConverterException, BasicFormattingException);

    namespace private_
    {
        template<std::integral Int, CharacterLiteral CharT>
        class IntegralConverter
        {
        public:
            constexpr IntegralConverter(
                std::basic_string<CharT> &formatting_string_,
                Int number_,
                u16 notation_ = Notation::DECIMAL)
              : formatting_string{ formatting_string_ }, number{ number_ }, notation{ notation_ }
            {
                if (land(notation > Notation::HEXADECIMAL, notation < Notation::BINARY)) {
                    throw IntegralConverterException{ "Notation is out of range" };
                }

                convertNumber();
            }

        private:
            constexpr auto convertNumber() -> void
            {
                if (number == 0) {
                    formatting_string.push_back('0');
                    return;
                }

                processSign();
                fillBuffer();
                copyBuffer();
            }

            constexpr auto processSign() -> void
            {
                if (number < 0) {
                    formatting_string.push_back('-');
                    number = -number;
                }
            }

            constexpr auto fillBuffer() -> void
            {
                while (number != 0) {
                    conversion_buffer[--buffer_index] =
                        IntToHexadecimalChars<CharT>[static_cast<u16>(number % notation)];
                    number /= notation;
                }
            }

            constexpr auto copyBuffer() -> void
            {
                for (; buffer_index != maximum_digits; ++buffer_index) {
                    formatting_string.push_back(conversion_buffer[buffer_index]);
                }
            }

            constexpr static size_t maximum_digits = std::numeric_limits<Int>::digits10 + 1;

            std::basic_string<CharT> &formatting_string;
            size_t buffer_index{ maximum_digits };
            std::array<CharT, maximum_digits> conversion_buffer{};
            Int number;
            u16 notation;
        };
    }// namespace private_

    template<std::integral Int, CharacterLiteral CharT>
    constexpr auto convert(std::basic_string<CharT> &formatting_string, Int value) -> void
    {
        private_::IntegralConverter<Int, CharT>{ formatting_string, value };
    }
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_INT_HPP */
