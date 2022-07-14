#ifndef CERBERUS_PROJECT_EXCEPTION_HPP
#define CERBERUS_PROJECT_EXCEPTION_HPP

#include <exception>
#include <string_view>

#define CERBLIB_EXCEPTION(name, base_exception)                                                    \
    struct name : base_exception                                                                   \
    {                                                                                              \
        static_assert(std::is_base_of_v<cerb::CerberusException, base_exception>);                 \
                                                                                                   \
        name() = default;                                                                          \
                                                                                                   \
        explicit name(std::string_view message_) : base_exception(message_)                        \
        {}                                                                                         \
    }

namespace cerb
{
    class CerberusException : public std::exception
    {
    public:
        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            return message.data();
        }

        CerberusException() = default;

        explicit CerberusException(std::string_view message_) : message(message_)
        {}

    private:
        std::string_view message{};
    };

    CERBLIB_EXCEPTION(LogicError, CerberusException);
    CERBLIB_EXCEPTION(OutOfRange, CerberusException);
    CERBLIB_EXCEPTION(KeyNotFound, CerberusException);
}// namespace cerb

#endif /* CERBERUS_PROJECT_EXCEPTION_HPP */
