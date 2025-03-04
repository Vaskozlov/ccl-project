#include <ccl/codegen/basic_codegen.hpp>
#include <ccl/lexer/analyzer_generator/static_generator.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/raii.hpp>

namespace ccl::lexer::gen
{
    using codegen::endl;
    using codegen::pop_scope;
    using codegen::push_scope;

    constexpr static auto IncludedHeaders = []() {
        std::array<isl::string_view, 3> result = {
            "<ccl/handler/cmd.hpp>", "<ccl/lexer/tokenizer.hpp>", "<isl/static_flat_map.hpp>"};

        std::ranges::sort(result);
        return result;
    }();

    constexpr static auto BuiltinRules =
        isl::StaticFlatmap<isl::string_view, SmallId, ReservedTokenMaxValue + 1>{
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
                auto exception = text::TextIteratorException{
                    ExceptionCriticality::CRITICAL,
                    AnalysisStage::PARSING,
                    name.getLocation(),
                    filename,
                    name_str.size(),
                    static_cast<std::string>(name.getInlineRepr()),
                    "unrecognizable directive",
                };

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

            codeGenerator << "enum " << enumName << " : ccl::SmallId;";
        }

        codeGenerator
            << fmt::format(
                   "template<>\n"
                   "CCL_DECL auto ccl::lexer::lexerEnumToString<{0}::{1}>(SmallId value) -> "
                   "std::string;",
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

        auto output_rule = [this](const isl::string_view value) {
            codeGenerator << endl << '{' << enumName << "::" << value;
            codeGenerator << ", " << '\"' << value << "\"},";
        };

        auto rule_defined = [&generated_rules](const parser::CcllParser::Rule &rule) {
            return generated_rules.contains(rule.name);
        };

        const auto &rules = ccllParser.getRules();

        codeGenerator << fmt::format(
            "inline constexpr isl::StaticFlatmap<ccl::SmallId, isl::string_view, {}> "
            "ToString{}Token\n",
            BuiltinRules.size() + rules.size(), variableName);
        codeGenerator << '{' << push_scope;

        for (const auto &rule : BuiltinRules | std::views::keys) {
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
        codeGenerator << "enum " << enumName << " : ccl::SmallId {";
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
        const auto &rules = ccllParser.getRules();

        const auto output_enum_case = [this](const isl::string_view name, const SmallId id) {
            codeGenerator << endl << name << " = " << fmt::to_string(id) << "U,";
        };

        for (const auto &[rule_name, rule_id] : BuiltinRules) {
            generated_enum_cases.emplace(rule_name);
            output_enum_case(rule_name, rule_id);
        }

        for (const parser::CcllParser::Rule &rule : rules) {
            if (generated_enum_cases.contains(rule.name)) {
                continue;
            }

            generated_enum_cases.emplace(rule.name);
            output_enum_case(rule.name, rule.id);
        }
    }

    auto StaticGenerator::generateVariable() -> void
    {
        generateLexicalAnalyzer();
        generateRuleNames();
    }

    auto StaticGenerator::generateFunctionDefinition() -> void
    {
        codeGenerator << "inline auto get" << variableName
                      << "() -> const ccl::lexer::LexicalAnalyzer & {";

        codeGenerator << push_scope << endl;
    }

    auto StaticGenerator::generateLexicalAnalyzerVariable() -> void
    {
        codeGenerator << "// NOLINTNEXTLINE" << endl;
        codeGenerator << "const static auto " << variableName << " = ccl::lexer::LexicalAnalyzer{";
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


    auto StaticGenerator::generateLexicalAnalyzer() -> void
    {
        generateFunctionDefinition();

        const auto function_definition = isl::Raii{[this]() {
            codeGenerator << pop_scope << endl;
            codeGenerator << '}' << endl << endl;
        }};

        generateLexicalAnalyzerVariable();

        codeGenerator << "return " << variableName << ';';
    }

    auto StaticGenerator::generateRules() -> void
    {
        for (const parser::CcllParser::Rule &rule : ccllParser.getRules()) {
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
            "CCL_DECL auto lexerEnumToString<{0}::{1}>(SmallId value) -> std::string ",
            nameSpace, enumName);

        codeGenerator << "{" << endl << push_scope;

        const auto function_scope = isl::Raii{[this] {
            codeGenerator << pop_scope << endl << '}';
        }};

        codeGenerator << fmt::format(
            "return static_cast<std::string>({}::ToString{}Token[value]);", nameSpace,
            variableName);
    }
}// namespace ccl::lexer::gen
