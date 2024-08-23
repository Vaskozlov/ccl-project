#ifndef CCL_PROJECT_LL_GSS_HPP
#define CCL_PROJECT_LL_GSS_HPP

#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/ll/sppf_node.hpp>
#include <deque>

namespace ccl::parser::ll
{
    class GSS
    {
    public:
        struct Node
        {
            std::vector<Node *> previous;
            SPPFNode sppfNode;
            SmallId inputPosition{};

            auto addParent(Node *parent) -> void
            {
                if (auto it = std::ranges::find(previous, parent); it == previous.end()) {
                    previous.emplace_back(parent);
                }
            }
        };

        struct PassedDescriptor
        {
            SPPFNode sppfNode;
            SmallId inputPosition;

            auto operator<=>(const PassedDescriptor &) const -> std::weak_ordering = default;
        };

        struct Descriptor
        {
            Node *stack;
            SmallId inputPosition;

            auto operator<=>(const Descriptor &other) const -> std::weak_ordering = default;
        };

        struct Level : std::vector<std::unique_ptr<Node>>
        {
            using vector::vector;

            [[nodiscard]] auto
                findNode(const SPPFNode &sppf_node) const CCL_LIFETIMEBOUND -> Node *;
        };

        using Levels = std::vector<Level>;

    private:
        Levels levels;
        std::deque<Descriptor> terminalDescriptors;
        std::deque<Descriptor> nonTerminalDescriptors;
        std::set<PassedDescriptor> passed;
        const GrammarStorage *storage{};
        lexer::Tokenizer *tokenizer{};
        SmallId globalInputPosition{};

    public:
        explicit GSS(const GrammarStorage *grammar_storage, lexer::Tokenizer *tokenizer_ptr)
          : storage{grammar_storage}
          , tokenizer{tokenizer_ptr}
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
        }

        auto
            pop(const Descriptor &descriptor,
                const std::function<std::string(SmallId)> &id_to_string_converter) -> void;

        auto
            add(Descriptor descriptor,
                const std::function<std::string(SmallId)> &id_to_string_converter) -> void;

        auto getDescriptor() -> Descriptor
        {
            if (nonTerminalDescriptors.empty()) {
                auto descriptor = terminalDescriptors.front();
                terminalDescriptors.pop_front();
                return descriptor;
            }

            if (terminalDescriptors.empty()) {
                auto descriptor = nonTerminalDescriptors.front();
                nonTerminalDescriptors.pop_front();
                return descriptor;
            }

            if (nonTerminalDescriptors.front().inputPosition <=
                terminalDescriptors.front().inputPosition) {
                auto descriptor = nonTerminalDescriptors.front();
                nonTerminalDescriptors.pop_front();
                return descriptor;
            }

            auto descriptor = terminalDescriptors.front();
            terminalDescriptors.pop_front();
            return descriptor;
        }

        auto createNode(Node *parent, const SPPFNode &sppf_node, SmallId input_position)
            CCL_LIFETIMEBOUND -> Node *;

        auto createNode(
            const std::vector<Node *> &parents,
            const SPPFNode &sppf_node,
            SmallId input_position) CCL_LIFETIMEBOUND -> Node *;

    private:
        auto getLevel(SmallId input_position) -> Level &;
    };
}// namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_GSS_HPP */
