#ifndef CCL_PROJECT_CMD_HANDLER_HPP
#define CCL_PROJECT_CMD_HANDLER_HPP

#include <ccl/handler/exception_handler.hpp>
#include <ccl/text/text_iterator.hpp>
#include <fmt/color.h>
#include <fmt/format.h>
#include <iostream>

namespace ccl::handler
{
    struct Cmd : ExceptionHandler
    {
        using ExceptionHandler::ExceptionT;

        Cmd() noexcept = default;

        static auto instance() -> Cmd &;

    private:
        auto onHandle(const ExceptionT *instance) -> void override;

        template<fmt::color Color, typename HandleType>
        auto formatAndPrint(const ExceptionT *value, HandleType &&handle_type) -> void;
    };
}// namespace ccl::handler

#endif /* CCL_PROJECT_CMD_HANDLER_HPP */
