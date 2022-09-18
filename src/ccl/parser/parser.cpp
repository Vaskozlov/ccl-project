#include <ccl/parser/parser.hpp>
#include <utility>

namespace ccl::parser
{
    using namespace std::string_literals;

    ParsingRules::ParsingRules(
        std::vector<ParsingRule>
            rules_,
        std::unordered_map<size_t, size_t>
            precedence_table_,
        std::string_view (*token_to_string_)(size_t))
      : precedence_table(std::move(precedence_table_)), token_to_string(token_to_string_)
    {
        initializeTable(rules_);
        reduceTableConflicts();
        fmt::print("----------------------------------------------------------------\n");
    }

    auto ParsingRules::initializeTable(std::vector<ParsingRule> &table_rules) -> void
    {
        for (auto &rule : table_rules) {
            auto &ids_to_construct = rule.ids_to_construct;
            std::ranges::reverse(ids_to_construct);

            if (ids_to_construct.empty()) {
                continue;
            }

            auto first_id = ids_to_construct.at(0);
            rules[first_id].push_back(rule);
        }
    }

    auto ParsingRules::reduceTableConflicts() -> void
    {
        for (auto &[id, vec_rules] : rules) {
            for (auto &rule : vec_rules) {

                const auto &rule_ids = rule.ids_to_construct;
                auto last_id = rule_ids.front();

                if (needToReduce(last_id)) {
                    reduceConflict(last_id, rule);
                }
            }
        }
    }

    auto ParsingRules::needToReduce(size_t last_id) const -> bool
    {
        return rules.contains(last_id) and rules.at(last_id).size() > 1;
    }

    auto ParsingRules::reduceConflict(size_t entry_id, ParsingRule &rule) -> void
    {
        auto &vec_of_rules = rules[entry_id];

        for (auto &rule_to_compare : vec_of_rules) {
            if (rule.ids_to_construct != rule_to_compare.ids_to_construct) {
                auto mismatch =
                    std::ranges::mismatch(rule.ids_to_construct, rule_to_compare.ids_to_construct);

                auto first_id = *mismatch.in1;
                auto second_id = *mismatch.in2;

                completeReduction(first_id, second_id, rule);
            }
        }
    }

    auto ParsingRules::completeReduction(size_t first_id, size_t second_id, ParsingRule &rule)
        -> void
    {
        if (precedence_table.contains(first_id) and precedence_table.contains(second_id)) {
            auto first_precedence = precedence_table.at(first_id);
            auto second_precedence = precedence_table.at(second_id);

            if (first_precedence < second_precedence) {
                rule.can_not_be_followed.insert(second_id);
            }

            fmt::print(
                "{} {}\n",
                toString(rule.ids_to_construct, [](size_t id) { return id; }),
                toString(rule.can_not_be_followed, [](size_t id) { return id; }));
        } else {
            fmt::print(
                "Unable to solve reduce/reduce conflict for {} and {}, because precedence table "
                "does not contain enough information\n",
                token_to_string(first_id),
                token_to_string(second_id));
        }
    }

    auto ParsingRules::Parser::parse() -> void
    {
        auto &token = tokenizer.yield();

        while (token) {
            stack.push_front(std::make_unique<TokenNode>(token));
            scanWithRules();
            token = tokenizer.yield();
        }

        stack[0]->print();
    }

    // NOLINTNEXTLINE (recursion)
    auto ParsingRules::Parser::scanWithRules() -> void
    {
        auto first_id = stack.front()->getId();

        if (not rules.contains(first_id)) {
            return;
        }

        fmt::print("{}\n", toString(stack, [](const NodePtr &node) { return node->getId(); }));

        for (const auto &rule : rules.at(first_id)) {
            if (applyRule(rule)) {
                return scanWithRules();
            }
        }
    }

    auto ParsingRules::Parser::applyRule(const ParsingRule &rule) -> bool
    {
        auto &future_token = tokenizer.futureToken();
        auto future_id = future_token.getId();

        if (shouldSkipRule(rule, future_id)) {
            return false;
        }

        if (matchesRule(rule)) {
            stack.push_front(rule.constructor(StackGetter{ stack }));
            return true;
        }

        return false;
    }

    auto ParsingRules::Parser::matchesRule(const ParsingRule &rule) const -> bool
    {
        auto stack_state = stack.begin();
        auto rule_length = rule.ids_to_construct.size();
        auto match_begin = rule.ids_to_construct.begin();
        auto matching_end = rule.ids_to_construct.begin() + static_cast<long>(rule_length);

        return std::all_of(match_begin, matching_end, [&stack_state](auto id) {
            return (*stack_state++)->getId() == id;
        });
    }

    auto ParsingRules::Parser::shouldSkipRule(const ParsingRule &rule, size_t future_id) const
        -> bool
    {
        const auto &can_not_be_followed = rule.can_not_be_followed;

        return can_not_be_followed.contains(future_id) ||
               stack.size() < rule.ids_to_construct.size();
    }

    auto ParsingRules::tokenToString(size_t id) const -> std::string
    {
        if (token_to_string != nullptr) {
            return std::string(token_to_string(id));
        }

        return fmt::format("{}\n", id);
    }

    auto ParsingRules::Parser::tokenToString(size_t id) const -> std::string
    {
        if (token_to_string != nullptr) {
            return std::string(token_to_string(id));
        }

        return fmt::format("{}\n", id);
    }

    template<typename T, typename Pred>
    auto ParsingRules::toString(const T &container, Pred &&function) const -> std::string
    {
        if (container.begin() == container.end()) {
            return "[]"s;
        }

        auto result = "["s;
        auto begin = container.begin();

        result.append(tokenToString(function(*begin)));

        for (++begin; begin != container.end(); ++begin) {
            result.append(", ");
            result.append(tokenToString(function(*begin)));
        }

        result.push_back(']');
        return result;
    }

    template<typename T, typename Pred>
    auto ParsingRules::Parser::toString(const T &container, Pred &&function) const -> std::string
    {
        if (container.begin() == container.end()) {
            return "[]"s;
        }

        auto result = "["s;
        auto begin = container.begin();

        result.append(tokenToString(function(*begin)));

        for (++begin; begin != container.end(); ++begin) {
            result.append(", ");
            result.append(tokenToString(function(*begin)));
        }

        result.push_back(']');
        return result;
    }
}// namespace ccl::parser
