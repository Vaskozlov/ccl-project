#ifndef CCL_PROJECT_LL_GSS_HPP
#define CCL_PROJECT_LL_GSS_HPP

#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/ll/gss/descriptor.hpp>
#include <ccl/parser/ll/gss/level.hpp>
#include <ccl/parser/ll/gss/passed_descriptor.hpp>
#include <deque>

namespace ccl::parser::ll
{
    class GSS
    {
    public:
        using Levels = std::vector<Level>;

    private:
        Levels levels;
        std::deque<Descriptor> terminalDescriptors;
        std::deque<Descriptor> nonTerminalDescriptors;
        std::array<ankerl::unordered_dense::set<PassedDescriptor>, 2> passed;
        const GrammarStorage *storage{};
        SmallId globalInputPosition{};

    public:
        explicit GSS(const GrammarStorage *grammar_storage)
          : storage{grammar_storage}
        {}

        [[nodiscard]] auto getGlobalInputPosition() const noexcept -> SmallId
        {
            return globalInputPosition;
        }

        [[nodiscard]] auto getLevels() const noexcept CCL_LIFETIMEBOUND -> const Levels &
        {
            return levels;
        }

        [[nodiscard]] auto hasDescriptors() const noexcept -> bool
        {
            return !terminalDescriptors.empty() || !nonTerminalDescriptors.empty();
        }

        auto nextWord() -> void
        {
            ++globalInputPosition;
            passed.at(globalInputPosition % 2).clear();
        }

        auto pop(const Descriptor &descriptor) -> void;

        auto add(Descriptor descriptor) -> void;

        auto getDescriptor() -> Descriptor;

        auto createNode(Node *parent, SPPFNode sppf_node, SmallId input_position)
            CCL_LIFETIMEBOUND -> Node *;

        auto createNode(
            const std::vector<Node *> &parents,
            SPPFNode sppf_node,
            SmallId input_position) CCL_LIFETIMEBOUND -> Node *;

    private:
        auto getLevel(SmallId input_position) -> Level &;
    };
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_GSS_HPP */
