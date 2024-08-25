#ifndef CCL_PROJECT_LL_GSS_HPP
#define CCL_PROJECT_LL_GSS_HPP

#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/ast/allocator.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/ll/sppf_node.hpp>
#include <deque>

namespace ccl::parser::ll
{
    struct PassedDescriptor
    {
        SPPFNode sppfNode;
        SmallId inputPosition{};

        auto operator<=>(const PassedDescriptor &) const -> std::weak_ordering = default;
    };
}// namespace ccl::parser::ll

template<>
struct ankerl::unordered_dense::hash<ccl::parser::ll::PassedDescriptor>
{
    using is_avalanching = void;

    [[nodiscard]] auto
        operator()(const ccl::parser::ll::PassedDescriptor &descriptor) const noexcept -> auto
    {
        return detail::wyhash::mix(
            descriptor.inputPosition,
            ankerl::unordered_dense::hash<ccl::parser::SPPFNode>{}(descriptor.sppfNode));
    }
};

namespace ccl::parser::ll
{
    struct Node
    {
        std::vector<Node *> previous;
        SPPFNode sppfNode;
        SmallId inputPosition{};

        auto addParent(Node *parent) -> void
        {
            for (auto *ptr : previous) {
                if (ptr == parent) {
                    return;
                }
            }

            previous.emplace_back(parent);
        }
    };

    struct Descriptor
    {
        Node *stack;
        SmallId inputPosition;

        auto operator<=>(const Descriptor &other) const -> std::weak_ordering = default;
    };

    namespace detail
    {
        // NOLINTNEXTLINE
        extern isl::FixedSizeAllocator<sizeof(Node), alignof(Node)> NodeAllocator;
    }// namespace detail

    using UniqueGssNodePtr = isl::UniquePtr<Node, std::addressof(detail::NodeAllocator)>;

    class GSS
    {
    public:
        struct Level : std::vector<UniqueGssNodePtr>
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
