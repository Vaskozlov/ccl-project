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
        CCL_DECL static auto failure() noexcept -> ParsingResult
        {
            return ParsingResult{std::numeric_limits<std::size_t>::max(), nullptr};
        }

        CCL_DECL explicit ParsingResult(
            std::size_t bytesPassed, std::unique_ptr<parser::ast::Node> constructed_node) noexcept
          : ScanResult{bytesPassed}
          , node{std::move(constructed_node)}
        {}

        CCL_DECL auto getNode() CCL_LIFETIMEBOUND -> parser::ast::Node *
        {
            return node.get();
        }

        CCL_DECL auto getAndReleaseNode() -> std::unique_ptr<parser::ast::Node>
        {
            return std::move(node);
        }

        CCL_DECL auto getNode() const CCL_LIFETIMEBOUND -> const parser::ast::Node *
        {
            return node.get();
        }

        CCL_DECL auto orElse(auto &&function) const CCL_LIFETIMEBOUND -> const ParsingResult &
        {
            if (isFailure()) {
                return function();
            }

            return *this;
        }
    };
}// namespace ccl::lexer

#endif /* CCL_PROJECT_PARSING_RESULT_HPP */
