#ifndef CCL_PROJECT_LL_GSS_HPP
#define CCL_PROJECT_LL_GSS_HPP

#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/ll/sppf_node.hpp>
#include <deque>

namespace ccl::parser::ll {
    class GSS {
    public:
        struct Node {
            std::vector<Node *> previous;
            SPPFNode sppfNode;
            SmallId inputPosition{};

            auto addPrevious(Node* parent) -> void {
                if (auto it = std::ranges::find(previous, parent); it == previous.end()) {
                    previous.emplace_back(parent);
                }
            }
        };

        struct PassedDescriptor {
            SPPFNode sppfNode;
            SmallId inputPosition;

            auto operator<=>(const PassedDescriptor&) const -> std::weak_ordering = default;
        };

        struct Descriptor {
            Node* stack;
            SmallId inputPosition;

            auto operator<=>(const Descriptor&other) const -> std::weak_ordering = default;
        };

        struct Level : std::vector<std::unique_ptr<Node>> {
            using vector::vector;

            [[nodiscard]] auto findNode(const SPPFNode&sppf_node) const -> Node*;
        };

        using Levels = std::vector<Level>;

    private:
        Levels levels;
        std::deque<Descriptor> descriptors;
        std::set<PassedDescriptor> passed;
        SmallId globalInputPosition{};

    public:
        [[nodiscard]] auto getGlobalInputPosition() const noexcept -> SmallId {
            return globalInputPosition;
        }

        [[nodiscard]] auto getLevels() const noexcept -> const Levels& {
            return levels;
        }

        [[nodiscard]] auto hasDescriptors() const noexcept -> bool {
            return !descriptors.empty();
        }

        auto nextWord() -> void {
            ++globalInputPosition;
        }

        auto pop(const Descriptor&descriptor) -> void;

        auto add(Descriptor descriptor) -> void;

        auto getDescriptor() -> Descriptor {
            auto descriptor = descriptors.front();
            descriptors.pop_front();

            return descriptor;
        }

        auto createNode(Node* parent, const SPPFNode&sppf_node, SmallId input_position) -> Node*;
    };
} // namespace ccl::parser::ll

#endif /* CCL_PROJECT_LL_GSS_HPP */
