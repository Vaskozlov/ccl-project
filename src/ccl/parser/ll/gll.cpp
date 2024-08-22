#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/ll/gll.hpp>
#include <ccl/parser/ll/gss.hpp>
#include <ccl/parser/lr/lr_item.hpp>

namespace ccl::parser {
    static auto debugGll(ll::GSS&gss, auto&&function) -> void {
        auto result = std::vector<ast::Node *>{};

        for (const auto&level: gss.getLevels()) {
            for (const auto&node: level) {
                for (auto* n: node->sppfNode.nodes) {
                    result.emplace_back(n);
                }
            }
        }

        auto tree_repr = dot::createDotRepresentation(result, function);
        isl::io::writeToFile(std::filesystem::path("/Volumes/ramdisk").append("gll.dot"), tree_repr);
    }

    GllParser::GllParser(
        SmallId start_symbol, const GrammarStorage&grammar_storage,
        const std::function<std::string(SmallId)>&id_to_string_converter)
        : idToStringConverter{id_to_string_converter}
          , storage{grammar_storage}
          , grammarGoalSymbol{start_symbol} {
        const auto ll_generator = ll::LlParserGenerator{start_symbol, storage, idToStringConverter};
        table = ll_generator.createGllTable();
    }

    auto GllParser::parse(lexer::LexicalAnalyzer::Tokenizer&tokenizer) -> AmbiguousParsingResult {
        auto parsing_result = AmbiguousParsingResult{};
        auto* nodes_lifetime_manager = parsing_result.nodesLifetimeManager.get();
        auto* token = nodes_lifetime_manager->create<ast::TokenNode>(tokenizer.yield());

        auto gss = ll::GSS{};
        gss.nextWord();

        const auto start_rule = RuleWithDot{
            .rule = std::addressof(storage.at(grammarGoalSymbol).front()),
            .dotPosition = 0,
        };

        const auto start_sppf_node = SPPFNode{.rule = start_rule};
        auto* start_node = gss.createNode(nullptr, start_sppf_node, 0);

        gss.add({
            .stack = start_node,
            .inputPosition = 0,
        });

        while (gss.hasDescriptors()) {
            auto descriptor = gss.getDescriptor();
            auto input_position = descriptor.inputPosition;

            if (input_position == gss.getGlobalInputPosition()) {
                token = nodes_lifetime_manager->create<ast::TokenNode>(tokenizer.yield());
                gss.nextWord();
            }

            // copy instead of making a reference?
            auto&sppf = descriptor.stack->sppfNode;

            auto lr_item = LrItem{};
            lr_item.dottedRule = sppf.rule;

            fmt::println("Rule: {}", LrItemPrintWrapper(lr_item, idToStringConverter));

            if (sppf.rule.isDotInTheEnd()) {
                gss.pop(descriptor);
                continue;
            }

            const auto focus = sppf.atDot();
            const auto token_type = token->getType();

            fmt::println(
                "Focus {}, token: {} = {}", idToStringConverter(focus),
                idToStringConverter(token_type), token->getToken().getRepr());

            if (focus == 0 && token_type == 0) {
                fmt::println("ACCEPTED");
                parsing_result.roots.emplace_back(descriptor.stack->sppfNode.nodes.front());
                debugGll(gss, idToStringConverter);
                continue;
            }

            if (storage.isTerminal(focus)) {
                if (focus != token_type) {
                    // ERROR STATE
                    continue;
                }

                sppf.next(token);
                descriptor.inputPosition += 1;
                gss.add(descriptor);
                continue;
            }

            const auto entry = TableEntry{
                .state = focus,
                .symbol = token_type,
            };

            auto rules_it = table.find(entry);

            if (rules_it == table.end()) {
                return parsing_result;
            }

            const auto&rules = rules_it->second;

            for (const auto* rule: rules) {
                auto new_sppf_node = SPPFNode{
                    .rule =
                    {
                        .rule = rule,
                        .dotPosition = 0,
                    },
                    .production = focus,
                };

                auto* new_node = gss.createNode(descriptor.stack, new_sppf_node, input_position);

                gss.add({
                    .stack = new_node,
                    .inputPosition = input_position,
                });
            }

            debugGll(gss, idToStringConverter);
        }

        return parsing_result;
    }
} // namespace ccl::parser
