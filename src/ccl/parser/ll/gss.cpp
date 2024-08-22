#include <ccl/parser/ll/gss.hpp>

namespace ccl::parser::ll {
    auto GSS::Level::findNode(const SPPFNode&sppf_node) const -> Node* {
        for (const auto&node: *this) {
            if (node->sppfNode == sppf_node) {
                return node.get();
            }
        }

        return nullptr;
    }

    auto GSS::pop(const Descriptor&descriptor) -> void {
        auto input_position = descriptor.inputPosition;

        if (input_position >= levels.size()) {
            levels.resize(input_position + 1);
        }

        auto&level = levels.at(input_position);
        auto&previous = descriptor.stack->previous;
        auto* new_node = descriptor.stack->sppfNode.build();

        for (auto* prev: previous) {
            auto sppf_copy = prev->sppfNode;
            sppf_copy.next(new_node);

            auto* node = level.findNode(sppf_copy);

            if (node == nullptr) {
                auto created_node = std::make_unique<Node>();

                created_node->sppfNode = sppf_copy;
                created_node->inputPosition = input_position;
                created_node->previous = prev->previous;

                node = created_node.get();
                level.emplace_back(std::move(created_node));
            }

            add({
                .stack = node,
                .inputPosition = descriptor.inputPosition,
            });
        }
    }

    auto GSS::add(Descriptor descriptor) -> void {
        auto passed_desc = PassedDescriptor{
            .sppfNode = descriptor.stack->sppfNode,
            .inputPosition = descriptor.inputPosition,
        };

        if (passed.contains(passed_desc)) {
            return;
        }

        passed.emplace(passed_desc);

        if (descriptor.inputPosition == globalInputPosition) {
            descriptors.emplace_back(descriptor);
        }
        else {
            descriptors.emplace_front(descriptor);
        }
    }

    auto GSS::createNode(Node* parent, const SPPFNode&sppf_node, SmallId input_position) -> Node* {
        if (input_position >= levels.size()) {
            levels.resize(input_position + 1);
        }

        auto&level = levels.at(input_position);
        auto* node = level.findNode(sppf_node);

        if (node == nullptr) {
            auto created_node = std::make_unique<Node>();

            created_node->sppfNode = sppf_node;
            created_node->inputPosition = input_position;
            created_node->previous.emplace_back(parent);

            node = created_node.get();
            level.emplace_back(std::move(created_node));
        }
        else {
            node->addPrevious(parent);
        }

        return node;
    }
} // namespace ccl::parser::ll
