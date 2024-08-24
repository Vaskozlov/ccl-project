#include <ccl/parser/ll/gss.hpp>

namespace ccl::parser::ll
{
    auto GSS::Level::findNode(const SPPFNode &sppf_node) const -> Node *
    {
        for (const auto &node : *this) {
            if (node->sppfNode == sppf_node) {
                return node.get();
            }
        }

        return nullptr;
    }

    auto GSS::pop(
        const Descriptor &descriptor,
        const std::function<std::string(SmallId)> &id_to_string_converter) -> void
    {
        const auto input_position = descriptor.inputPosition;
        auto constructed_node = descriptor.stack->sppfNode.build();

        for (Node *prev : descriptor.stack->previous) {
            auto new_sppf = prev->sppfNode;
            new_sppf.next(constructed_node);

            auto *new_node = createNode(prev->previous, new_sppf, input_position);

            add(
                {
                    .stack = new_node,
                    .inputPosition = input_position,
                },
                id_to_string_converter);
        }
    }

    auto GSS::add(
        Descriptor descriptor,
        const std::function<std::string(SmallId)> &id_to_string_converter) -> void
    {
        auto inserted = false;
        auto *node = descriptor.stack;
        const auto input_position = descriptor.inputPosition;

        std::tie(std::ignore, inserted) = passed.at(input_position % 2)
                                              .emplace(
                                                  PassedDescriptor{
                                                      .sppfNode = node->sppfNode,
                                                      .inputPosition = node->inputPosition,
                                                  });

        if (!inserted) {
            fmt::println(
                "Rejecting descriptor: {} at {}",
                RuleWithDotPrintWrapper{
                    node->sppfNode.rule,
                    id_to_string_converter,
                },
                descriptor.inputPosition);
            return;
        }

        fmt::println(
            "Accepting descriptor: {} at {}",
            RuleWithDotPrintWrapper{
                node->sppfNode.rule,
                id_to_string_converter,
            },
            descriptor.inputPosition);

        const auto at_dot = descriptor.stack->sppfNode.rule.isDotInTheEnd()
                                ? 0
                                : descriptor.stack->sppfNode.atDot();

        if (descriptor.inputPosition == globalInputPosition) {
            if (storage->isTerminal(at_dot)) {
                terminalDescriptors.emplace_back(descriptor);
            } else {
                nonTerminalDescriptors.emplace_back(descriptor);
            }
        } else {
            if (storage->isTerminal(at_dot)) {
                terminalDescriptors.emplace_front(descriptor);
            } else {
                nonTerminalDescriptors.emplace_front(descriptor);
            }
        }
    }

    auto GSS::createNode(Node *parent, const SPPFNode &sppf_node, SmallId input_position) -> Node *
    {
        auto &level = getLevel(input_position);
        auto *node = level.findNode(sppf_node);

        if (node == nullptr) {
            auto constructed_node =
                std::make_unique<Node>(std::vector{parent}, sppf_node, input_position);

            node = constructed_node.get();
            level.emplace_back(std::move(constructed_node));

            return node;
        }

        node->addParent(parent);
        return node;
    }

    auto GSS::createNode(
        const std::vector<Node *> &parents,
        const SPPFNode &sppf_node,
        SmallId input_position) -> Node *
    {
        auto &level = getLevel(input_position);
        auto *node = level.findNode(sppf_node);

        if (node == nullptr) {
            auto constructed_node = std::make_unique<Node>(parents, sppf_node, input_position);

            node = constructed_node.get();
            level.emplace_back(std::move(constructed_node));

            return node;
        }

        for (auto *parent : parents) {
            node->addParent(parent);
        }

        return node;
    }


    auto GSS::getLevel(SmallId input_position) -> Level &
    {
        if (input_position >= levels.size()) {
            levels.resize(input_position + 1);
        }

        return levels.at(input_position);
    }
}// namespace ccl::parser::ll
