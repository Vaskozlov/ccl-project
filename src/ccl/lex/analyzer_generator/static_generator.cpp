#include <ccl/codegen/basic_codegen.hpp>
#include <ccl/lex/analyzer_generator/static_generator.hpp>
#include <ccl/raii.hpp>
#include <range/v3/view.hpp>

namespace ccl::lex::gen
{
    using codegen::endl;
    using codegen::pop_scope;
    using codegen::push_scope;

    constexpr static auto ShiftSize = sizeof(size_t) * 8 / 2;

    constexpr static auto IncludedHeaders = []() {
        std::array<string_view, 3> result = {
            "<ccl/handler/cmd.hpp>", "<ccl/lex/tokenizer.hpp>", "<ccl/flatmap.hpp>"};

        std::sort(result.begin(), result.end());
        return result;
    }();

    constexpr static auto BuiltinRules =
        StaticFlatmap<string_view, Id, 2>{{"EOI", 0}, {"BAD_TOKEN", 1}};

    StaticGenerator::StaticGenerator(Tokenizer &input_tokenizer)
      : tokenizer{input_tokenizer}
      , ccllParser{tokenizer}
    {
        generate();
    }

    auto StaticGenerator::loadDirectives() -> void
    {
        for (const auto &[name, value] : ccllParser.getDirectives()) {
            if ("VAR_NAME" == name) {
                variableName = value;
            } else if ("HANDLER" == name) {
                handler = value;
            } else if ("NAMESPACE" == name) {
                nameSpace = value;
            } else {
                fmt::print("unrecognizable directive: {}\n", name);
            }
        }

        if (enumName.empty()) {
            enumName = fmt::format("{}Token", variableName);
        }
    }

    auto StaticGenerator::generate() -> void
    {
        ccllParser.parse();

        loadDirectives();

        generateHeaderDefinition();
        generateNamespaceBegin();

        generateEnum();
        generateVariable();

        generateNamespaceEnd();
    }

    auto StaticGenerator::generateHeaderDefinition() -> void
    {
        codeGenerator << "#pragma once" << endl << endl;

        for (const auto &header_name : IncludedHeaders) {
            codeGenerator << "#include " << header_name << endl;
        }

        codeGenerator << endl;
    }

    auto StaticGenerator::generateNamespaceBegin() -> void
    {
        if (nameSpace.empty()) {
            return;
        }

        codeGenerator << "namespace " << nameSpace << endl << '{' << endl << push_scope;
    }

    auto StaticGenerator::generateNamespaceEnd() -> void
    {
        if (nameSpace.empty()) {
            return;
        }

        codeGenerator << pop_scope << endl << "}// namespace " << nameSpace << endl;
    }

    auto StaticGenerator::generateRuleNames() -> void
    {
        auto generated_rules = Set<string_view>{};

        auto output_rule = [this](string_view value) {
            codeGenerator << endl << '{' << enumName << "::" << value;
            codeGenerator << ", " << '\"' << value << "\"},";
        };

        auto undefined_rules = [&generated_rules](const parser::CcllParser::Rule &rule) {
            return !generated_rules.contains(rule.name);
        };

        const auto &rules = ccllParser.getRules();

        codeGenerator << fmt::format(
            "inline constexpr ccl::StaticFlatmap<ccl::Id, ccl::string_view, {}> ToString{}Token\n",
            BuiltinRules.size() + rules.size(), variableName);
        codeGenerator << '{' << push_scope;

        for (const auto &[rule, id] : BuiltinRules) {
            output_rule(rule);
        }

        for (const auto &rule : rules | ranges::views::filter(undefined_rules)) {
            generated_rules.emplace(rule.name);
            output_rule(rule.name);
        }

        codeGenerator << pop_scope << endl << "};";

        if (nameSpace.empty()) {
            codeGenerator << endl;
        }
    }

    auto StaticGenerator::generateEnum() -> void
    {
        auto enum_definition = Raii{
            [this]() {
                codeGenerator << "enum " << enumName << " : ccl::Id {";
                codeGenerator << push_scope;
            },
            [this]() {
                codeGenerator << pop_scope << endl;
                codeGenerator << "};" << endl << endl;
            }};

        generateEnumCases();
    }

    auto StaticGenerator::generateEnumCases() -> void
    {
        auto generated_blocks = Set<string_view>{};
        auto generated_enum_cases = Set<string_view>{};
        const auto &blocks = ccllParser.getBlocks();
        const auto &rules = ccllParser.getRules();

        auto undefined_blocks = [&generated_blocks](const auto &block) {
            auto &[block_name, _] = block;
            return !generated_blocks.contains(block_name);
        };

        auto undefined_enum_cases = [&generated_enum_cases](const parser::CcllParser::Rule &rule) {
            return !generated_enum_cases.contains(string_view{rule.name});
        };

        auto output_enum_case = [this](string_view name, Id id) {
            codeGenerator << endl << name << " = " << fmt::to_string(id) << ',';
        };

        for (const auto &[block_name, block_info] :
             blocks | ranges::views::filter(undefined_blocks)) {
            auto block_id = as<Id>(block_info.blockId) << ShiftSize;

            generated_blocks.emplace(block_name);
            output_enum_case(block_name, block_id);
        }

        for (const auto &[rule_name, rule_id] : BuiltinRules) {
            generated_enum_cases.emplace(rule_name);
            output_enum_case(rule_name, rule_id);
        }

        for (const auto &rule : rules | ranges::views::filter(undefined_enum_cases)) {
            auto id = (as<Id>(rule.blockId) << ShiftSize) + rule.id;

            generated_enum_cases.emplace(rule.name);
            output_enum_case(rule.name, id);
        }
    }

    auto StaticGenerator::generateVariable() -> void
    {
        generateLexicalAnalyzer();
        generateRuleNames();
    }

    auto StaticGenerator::generateLexicalAnalyzer() -> void
    {
        auto generator_definition = Raii{
            [this]() {
                codeGenerator << "// NOLINTNEXTLINE" << endl;
                codeGenerator << "inline auto " << variableName << " = ccl::lex::LexicalAnalyzer{";
                codeGenerator << push_scope << endl;
                codeGenerator << handler << ',' << endl;
                codeGenerator << '{' << push_scope;
            },
            [this]() {
                codeGenerator << pop_scope << endl;
                codeGenerator << '}';
                codeGenerator << pop_scope << endl;
                codeGenerator << "};" << endl << endl;
            }};

        generateRules();
    }

    auto StaticGenerator::generateRules() -> void
    {
        const auto &rules = ccllParser.getRules();

        for (const auto &rule : rules) {
            codeGenerator << endl << '{';
            codeGenerator << enumName << "::" << rule.name << ", ";
            codeGenerator << "R\"( ";
            codeGenerator << rule.definition;
            codeGenerator << " )\"";
            codeGenerator << "},";
        }
    }
}// namespace ccl::lex::gen
