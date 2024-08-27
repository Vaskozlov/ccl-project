#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/ll/gll.hpp>
#include <ccl/parser/ll/gss/gss.hpp>
#include <ccl/parser/lr/lr_item.hpp>
#include <isl/io.hpp>

namespace ccl::parser
{
    static auto debugGll(ll::GSS &gss, auto &&function) -> void
    {
        auto result = std::vector<ast::Node *>{};

        for (const auto &level : gss.getLevels()) {
            for (const auto &node : level) {
                for (const auto &n : node->sppfNode.nodes) {
                    result.emplace_back(n.get());
                }
            }
        }

        auto tree_repr = dot::createDotRepresentation(result, function);
        isl::io::writeToFile(
            std::filesystem::path("/Volumes/ramdisk").append("gll.dot"), tree_repr);
    }

    GllParser::GllParser(
        SmallId start_symbol, const GrammarStorage &grammar_storage,
        const std::function<std::string(SmallId)> &id_to_string_converter)
      : table{ll::GllParserGenerator{grammar_storage, id_to_string_converter}.createGllTable()}
      , idToStringConverter{id_to_string_converter}
      , storage{grammar_storage}
      , grammarGoalSymbol{start_symbol}
    {}

    auto GllParser::parse(lexer::Tokenizer &tokenizer) const -> AmbiguousParsingResult
    {
        auto parsing_result = AmbiguousParsingResult{.algorithmName = "GLL"};
        auto token = ast::SharedNode<ast::Terminal>{tokenizer.yield()};
        auto gss = ll::GSS{std::addressof(storage)};
        gss.nextWord();

        auto start_rule = storage.at(grammarGoalSymbol).front();
        start_rule.emplace_back(0);

        const auto start_rule_with_dot = RuleWithDot{
            .rule = &start_rule,
            .dotPosition = 0,
        };

        auto *start_node = gss.createNode(
            nullptr,
            SPPFNode{
                .nodes = {},
                .rule = start_rule_with_dot,
                .production = grammarGoalSymbol,
            },
            0);

        gss.add({
            .stack = start_node,
            .inputPosition = 0,
        });

        while (gss.hasDescriptors()) {
            auto descriptor = gss.getDescriptor();
            auto input_position = descriptor.inputPosition;

            if (input_position == gss.getGlobalInputPosition()) {
                token = ast::SharedNode<ast::Terminal>(tokenizer.yield());
                gss.nextWord();
            }

            auto &sppf = descriptor.stack->sppfNode;

            if (sppf.rule.isDotInTheEnd()) {
                gss.pop(descriptor);
                continue;
            }

            const auto focus = sppf.atDot();
            const auto token_type = token->getType();

            // fmt::println(
            // "Focus {}, token: {} = {}", idToStringConverter(focus),
            // idToStringConverter(token_type), token->getToken().getRepr());

            if (focus == 0 && token_type == 0) {
                parsing_result.roots.emplace_back(sppf.nodes.front());
                // debugGll(gss, idToStringConverter);
                continue;
            }

            if (storage.isTerminal(focus)) {
                if (focus != token_type) {
                    continue;
                }

                sppf.next(token);
                descriptor.inputPosition += 1;
                gss.add(descriptor);

                continue;
            }

            auto rules_it = table.find({
                .state = focus,
                .symbol = token_type,
            });

            if (rules_it == table.end()) {
                continue;
            }

            for (const auto *rule : rules_it->second) {
                auto *new_node = gss.createNode(
                    descriptor.stack,
                    SPPFNode{
                        .nodes = {},
                        .rule = {.rule = rule, .dotPosition = 0},
                        .production = focus,
                    },
                    input_position);

                gss.add({
                    .stack = new_node,
                    .inputPosition = input_position,
                });
            }

            // debugGll(gss, idToStringConverter);
        }

        return parsing_result;
    }
}// namespace ccl::parser
