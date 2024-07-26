#include <ccl/codegen/basic_codegen.hpp>
#include <ccl/lexer/analyzer_generator/static_generator.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/raii.hpp>

namespace ccl::lexer::gen
{
    using codegen::endl;
    using codegen::pop_scope;
    using codegen::push_scope;

    constexpr static auto ShiftSize = sizeof(std::size_t) * 8 / 2;

    constexpr static auto IncludedHeaders = []() {
        std::array<isl::string_view, 3> result = {
            "<ccl/handler/cmd.hpp>", "<ccl/lexer/tokenizer.hpp>", "<isl/flatmap.hpp>"};

        std::ranges::sort(result);
        return result;
    }();

    constexpr static auto BuiltinRules =
        isl::StaticFlatmap<isl::string_view, Id, ReservedTokenMaxValue + 1>{
            {"EOI", 0},
            {"BAD_TOKEN", 1},
            {"CUT", 2}};

    StaticGenerator::StaticGenerator(Tokenizer &input_tokenizer)
      : tokenizer{input_tokenizer}
      , ccllParser{tokenizer}
    {
        generate();
    }

    auto StaticGenerator::loadDirectives() -> void
    {
        for (const auto &[name, value] : ccllParser.getDirectives()) {
            const auto name_str = name.getRepr();
            auto value_str = isl::as<std::string>(value);

            if (name_str == "VAR_NAME") {
                variableName = std::move(value_str);
            } else if (name_str == "HANDLER") {
                handler = std::move(value_str);
            } else if (name_str == "NAMESPACE") {
                nameSpace = std::move(value_str);
            } else {
                auto exception = text::TextIteratorException(
                    ExceptionCriticality::CRITICAL, AnalysisStage::PARSING, name.getLocation(),
                    name_str.size(), name.getInlineRepr(), "unrecognizable directive");
                tokenizer.getHandler().handle(exception);
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
        generateToStringFunction();
    }

    auto StaticGenerator::generateHeaderDefinition() -> void
    {
        codeGenerator << "#pragma once" << endl << endl;

        for (const isl::string_view &header_name : IncludedHeaders) {
            codeGenerator << "#include " << header_name << endl;
        }

        codeGenerator << endl;

        {
            codeGenerator << "namespace " << nameSpace << '{' << push_scope << endl;

            const auto namespace_scope = isl::Raii{[this] {
                codeGenerator << pop_scope << endl << '}' << endl << endl;
            }};

            codeGenerator << "enum " << enumName << " : ccl::Id;";
        }

        codeGenerator
            << fmt::format(
                   "template<>\n"
                   "CCL_DECL auto ccl::lexer::lexerEnumToString<{0}::{1}>({0}::{1} value) -> "
                   "isl::string_view;",
                   nameSpace, enumName)
            << endl
            << endl;
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
        auto generated_rules = std::set<isl::string_view>{};

        auto output_rule = [this](isl::string_view value) {
            codeGenerator << endl << '{' << enumName << "::" << value;
            codeGenerator << ", " << '\"' << value << "\"},";
        };

        auto rule_defined = [&generated_rules](const parser::CcllParser::Rule &rule) {
            return generated_rules.contains(rule.name);
        };

        const auto &rules = ccllParser.getRules();

        codeGenerator << fmt::format(
            "inline constexpr isl::StaticFlatmap<ccl::Id, isl::string_view, {}> ToString{}Token\n",
            BuiltinRules.size() + rules.size(), variableName);
        codeGenerator << '{' << push_scope;

        for (const auto &[rule, id] : BuiltinRules) {
            output_rule(rule);
        }

        for (const auto &rule : rules) {
            if (rule_defined(rule)) {
                continue;
            }

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
        codeGenerator << "enum " << enumName << " : ccl::Id {";
        codeGenerator << push_scope;

        auto enum_definition = isl::Raii{[this]() {
            codeGenerator << pop_scope << endl;
            codeGenerator << "};" << endl << endl;
        }};

        generateEnumCases();
    }

    auto StaticGenerator::generateEnumCases() -> void
    {
        auto generated_blocks = std::set<isl::string_view>{};
        auto generated_enum_cases = std::set<isl::string_view>{};
        const auto &blocks = ccllParser.getBlocks();
        const auto &rules = ccllParser.getRules();

        const auto output_enum_case = [this](isl::string_view name, Id id) {
            codeGenerator << endl << name << " = " << fmt::to_string(id) << "ULL,";
        };

        for (const auto &[block_name, block_info] : blocks) {
            if (generated_blocks.contains(block_name)) {
                continue;
            }

            auto block_id = isl::as<Id>(block_info.blockId) << ShiftSize;

            generated_blocks.emplace(block_name);
            output_enum_case(block_name, block_id);
        }

        for (const auto &[rule_name, rule_id] : BuiltinRules) {
            generated_enum_cases.emplace(rule_name);
            output_enum_case(rule_name, rule_id);
        }

        for (const parser::CcllParser::Rule &rule : rules) {
            if (generated_enum_cases.contains(rule.name)) {
                continue;
            }

            auto id = (isl::as<Id>(rule.blockId) << ShiftSize) + rule.id;

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
        codeGenerator << "// NOLINTNEXTLINE" << endl;
        codeGenerator << "inline auto " << variableName << " = ccl::lexer::LexicalAnalyzer{";
        codeGenerator << push_scope << endl;
        codeGenerator << handler << ',' << endl;
        codeGenerator << '{' << push_scope;

        const auto generator_definition = isl::Raii{[this]() {
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

        for (const parser::CcllParser::Rule &rule : rules) {
            codeGenerator << endl << '{';
            codeGenerator << enumName << "::" << rule.name << ", ";
            codeGenerator << "R\"( ";
            codeGenerator << rule.definition;
            codeGenerator << " )\"";
            codeGenerator << "},";
        }
    }

    auto StaticGenerator::generateToStringFunction() -> void
    {
        codeGenerator << endl;
        codeGenerator << "namespace ccl::lexer {" << endl << push_scope;

        const auto namespace_scope = isl::Raii{[this] {
            codeGenerator << pop_scope << endl << '}' << endl << endl;
        }};

        codeGenerator << fmt::format(
            "template<>\n"
            "CCL_DECL auto lexerEnumToString<{0}::{1}>({0}::{1} value) -> isl::string_view ",
            nameSpace, enumName);
        codeGenerator << "{" << endl << push_scope;

        const auto function_scope = isl::Raii{[this] {
            codeGenerator << pop_scope << endl << '}';
        }};

        codeGenerator << fmt::format("return {}::ToString{}Token[value];", nameSpace, variableName);
    }
}// namespace ccl::lexer::gen
