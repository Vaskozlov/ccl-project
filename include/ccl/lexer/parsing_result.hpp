#ifndef CCL_PROJECT_PARSING_RESULT_HPP
#define CCL_PROJECT_PARSING_RESULT_HPP

#include <ccl/lexer/scan_result.hpp>
#include <ccl/parser/ast/node.hpp>
#include <numeric>

namespace ccl::lexer
{
    class ParsingResult : public ScanResult
    {
    private:
        std::unique_ptr<parser::ast::Node> node;

    public:
        [[nodiscard]] constexpr static auto failure() noexcept -> ParsingResult
        {
            return ParsingResult{std::numeric_limits<std::size_t>::max(), nullptr};
        }

        [[nodiscard]] explicit ParsingResult(
            std::size_t bytesPassed, std::unique_ptr<parser::ast::Node> constructed_node) noexcept
          : ScanResult{bytesPassed}
          , node{std::move(constructed_node)}
        {}

        [[nodiscard]] auto getNode() CCL_LIFETIMEBOUND -> parser::ast::Node *
        {
            return node.get();
        }

        [[nodiscard]] auto getAndReleaseNode() -> std::unique_ptr<parser::ast::Node>
        {
            return std::move(node);
        }

        [[nodiscard]] auto getNode() const CCL_LIFETIMEBOUND -> const parser::ast::Node *
        {
            return node.get();
        }

        [[nodiscard]] auto orElse(auto &&function) const CCL_LIFETIMEBOUND -> const ParsingResult &
        {
            if (isFailure()) {
                return function();
            }

            return *this;
        }
    };
}// namespace ccl::lexer

#endif /* CCL_PROJECT_PARSING_RESULT_HPP */
