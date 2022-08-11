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
        template<typename... Ts>                                                                   \
        explicit name(Ts &&...args) : base_exception{ std::forward<Ts>(args)... }                  \
        {                                                                                          \
        }                                                                                          \
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
    CERBLIB_EXCEPTION(InvalidArgument, CerberusException);
    CERBLIB_EXCEPTION(UnrecoverableError, CerberusException);
}// namespace cerb

#endif /* CERBERUS_PROJECT_EXCEPTION_HPP */
