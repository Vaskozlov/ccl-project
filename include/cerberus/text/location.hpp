#ifndef CERBERUS_PROJECT_LOCATION_HPP
#define CERBERUS_PROJECT_LOCATION_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/text/typedefs.hpp>

namespace cerb::text
{
    template<CharacterLiteral CharT = char>
    class Location
    {
    public:
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return line;
        }

        CERBLIB_DECL auto getColumn() const -> size_t
        {
            return column;
        }

        CERBLIB_DECL auto getFilename() const -> const StrView<CharT> &
        {
            return filename;
        }

        constexpr auto next(CharT chr) -> void
        {
            if (chr == '\n') {
                nextLine();
            } else if (chr != 0) {
                nextChar();
            }
        }

        Location() = default;

        constexpr explicit Location(StrView<CharT> filename_, size_t line_ = 1, size_t column_ = 0)
          : filename{ filename_ }, line{ line_ }, column{ column_ }
        {}

    private:
        constexpr auto nextChar() -> void
        {
            ++column;
        }

        constexpr auto nextLine() -> void
        {
            column = 1;
            ++line;
        }

        StrView<CharT> filename{};
        size_t line{ 1 };
        size_t column{ 0 };
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_LOCATION_HPP */
