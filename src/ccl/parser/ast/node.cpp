#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/ast/node_of_nodes.hpp>

namespace ccl::parser::ast
{
    static auto operator*(std::string_view str, size_t repeat) -> std::string
    {
        auto result = std::string{};
        result.reserve(str.size() * repeat);

        for (auto i = isl::as<size_t>(0); i != repeat; ++i) {
            result.append(str);
        }

        return result;
    }

    auto Node::getPrintingPrefix(const std::string &prefix, bool is_left) -> std::string
    {
        return prefix + (is_left ? "├──" : "└──");
    }

    auto Node::expandPrefix(const std::string &prefix, bool is_left, size_t extra_expansion)
        -> std::string
    {
        static constexpr auto default_printing_shift = std::string_view{"   "};

        auto result = prefix + (is_left ? "│   " : "    ");
        result.append(default_printing_shift * extra_expansion);

        return result;
    }

    auto Node::cast(const ConversionTable &conversion_table, Node *node) -> void
    {
        auto *node_as_sequence = dynamic_cast<NodeOfNodes *>(node);

        if (node_as_sequence == nullptr) {
            return;
        }

        const auto conversion_function = conversion_table.at(node_as_sequence->getType());
        conversion_function(node_as_sequence);

        std::launder(node_as_sequence)->castChildren(conversion_table);
    }
}// namespace ccl::parser::ast