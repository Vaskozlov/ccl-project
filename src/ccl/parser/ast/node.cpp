#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/ast/non_terminal.hpp>

namespace ccl::parser::ast
{
    static auto repeatString(const std::string_view str, const std::size_t times) -> std::string
    {
        auto result = std::string{};
        result.reserve(str.size() * times);

        for (auto i = std::size_t{}; i != times; ++i) {
            result.append(str);
        }

        return result;
    }

    auto Node::getPrintingPrefix(const std::string &prefix, const bool is_left) -> std::string
    {
        return prefix + (is_left ? "├──" : "└──");
    }

    auto Node::expandPrefix(const std::string &prefix, const bool is_left, const std::size_t extra_expansion)
        -> std::string
    {
        static constexpr auto default_printing_shift = std::string_view{"   "};

        auto result = prefix + (is_left ? "│   " : "    ");
        result.append(repeatString(default_printing_shift, extra_expansion));

        return result;
    }

    auto Node::cast(const ConversionTable &conversion_table) -> void
    {
        auto *node_as_sequence = dynamic_cast<NonTerminal *>(this);

        if (node_as_sequence == nullptr) {
            return;
        }

        const auto conversion_function = conversion_table.at(node_as_sequence->getType());
        conversion_function(node_as_sequence);

        std::launder(node_as_sequence)->castChildren(conversion_table);
    }
}// namespace ccl::parser::ast