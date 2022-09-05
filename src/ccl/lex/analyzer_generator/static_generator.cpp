#include <ccl/lex/analyzer_generator/static_generator.hpp>

namespace ccl::lex::gen
{
    auto StaticGenerator::loadDirectives() -> void
    {
        for (auto &&directive : ccll_parser.directives) {
            if (directive.first == "FILENAME") {
                filename = directive.second;
            } else if (directive.first == "VAR_NAME") {
                variable_name = directive.second;
            } else if (directive.first == "HANDLER") {
                handler = directive.second;
            } else {
                fmt::print("unrecognizable directive: {}\n", directive.first);
            }
        }

        if (enum_name.empty()) {
            enum_name = fmt::format("Token_{}", variable_name);
        }
    }

    auto StaticGenerator::generateHeader() -> void
    {
        ccll_parser.parse();

        loadDirectives();
        generateHeaderDefinition();
        generateEnum();
        generateVariable();

        fmt::print("{}\n", generated_header);
    }

    auto StaticGenerator::generateHeaderDefinition() -> void
    {
        generated_header =
            "#pragma once\n\n"
            "#include <ccl/handler/cmd_handler.hpp>\n"
            "#include <ccl/lex/lexical_analyzer.hpp>\n\n";
    }

    auto StaticGenerator::generateEnum() -> void
    {
        generated_header.append(
            fmt::format("CCL_PREDEFINED_ENUM(\n    {},\n    size_t", enum_name));

        generateEnumCases();

        generated_header.append(");\n\n");
    }

    auto StaticGenerator::generateEnumCases() -> void
    {
        constexpr auto shift_size = 16;

        for (auto &&rule : ccll_parser.rules) {
            auto id = (rule.block_id << shift_size) | (rule.id);
            generated_header.append(fmt::format(",\n    {} = {}", rule.name, id));
        }
    }

    auto StaticGenerator::generateVariable() -> void
    {
        generated_header.append(fmt::format(
            "const static inline ccl::lex::LexicalAnalyzer {}(\n    {},\n    {{\n", variable_name,
            handler));

        generateRules();

        generated_header.append("    });\n\n");
    }

    auto StaticGenerator::generateRules() -> void
    {
        for (auto &&rule : ccll_parser.rules) {
            auto rule_name = fmt::format("{}::{}", enum_name, rule.name);
            generated_header.append(
                fmt::format("        {{ {}, R\"( {} )\" }},\n", rule_name, rule.definition));
        }
    }
}// namespace ccl::lex::gen
