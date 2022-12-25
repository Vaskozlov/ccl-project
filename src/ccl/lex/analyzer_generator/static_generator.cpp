#include <ccl/lex/analyzer_generator/static_generator.hpp>
#include <ranges>

namespace ccl::lex::gen
{
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
        generateNamespaceBegin(generatedHeader);

        generateEnum();
        generateVariable();

        generateNamespaceEnd(generatedHeader);

        generateSource();
    }

    auto StaticGenerator::generateSource() -> void
    {
        generatedSource = fmt::format("#include <{}/{}>\n\n", includeDirForSrc, filename);

        generateNamespaceBegin(generatedSource);
        generateLexicalAnalyzer(generatedSource);
        generateRuleNames(generatedSource);
        generateNamespaceEnd(generatedSource);
    }

    auto StaticGenerator::generateHeaderDefinition() -> void
    {
        generatedHeader =
            "#pragma once\n\n"
            "#include <ccl/handler/cmd_handler.hpp>\n"
            "#include <ccl/lex/tokenizer.hpp>\n\n";
    }

    auto StaticGenerator::generateNamespaceBegin(std::string &string) -> void
    {
        if (!nameSpace.empty()) {
            extraSpaces = "    ";
            string.append(fmt::format("namespace {}\n{{\n", nameSpace));
        }
    }

    auto StaticGenerator::generateNamespaceEnd(std::string &string) -> void
    {
        if (!nameSpace.empty()) {
            extraSpaces.clear();
            string.append(fmt::format("}}// namespace {}\n", nameSpace));
        }
    }

    auto StaticGenerator::generateRuleNames(std::string &string, std::string addition_flags) -> void
    {
        if (!addition_flags.empty() && ' ' != addition_flags.back()) {
            addition_flags.push_back(' ');
        }

        appendToStr(
            string,
            fmt::format(
                "{}const ccl::UnorderedMap<ccl::Id, ccl::string_view> ToString{}Token\n    {{\n",
                addition_flags, variableName));

        Set<string_view> generated_rules{};

        appendToStr(string, fmt::format("    {{ {0}::{1}, \"{1}\" }},\n", enumName, "EOI"));
        appendToStr(string, fmt::format("    {{ {0}::{1}, \"{1}\" }},\n", enumName, "BAD_TOKEN"));

        auto repeat_filter = [&generated_rules](const parser::CcllParser::Rule &rule) {
            return !generated_rules.contains(rule.name);
        };

        for (auto &&rule : ccllParser.getRules() | std::views::filter(repeat_filter)) {
            generated_rules.insert(rule.name);
            appendToStr(string, fmt::format("    {{ {0}::{1}, \"{1}\" }},\n", enumName, rule.name));
        }

        appendToStr(string, "};\n");
    }

    auto StaticGenerator::generateEnum() -> void
    {
        appendToStr(generatedHeader, fmt::format("{0}enum {1}: size_t {{", extraSpaces, enumName));

        generateEnumCases();

        generatedHeader.append("\n};\n\n");
    }

    auto StaticGenerator::generateEnumCases() -> void
    {
        constexpr auto shift_size = as<size_t>(16);

        Set<string_view> generated_cases{};
        Set<string_view> generated_blocks{};

        auto single_block_definition = [&generated_blocks](const auto &block) {
            const auto &block_name = block.first;
            return !generated_blocks.contains(block_name);
        };

        for (const auto &[block_name, block_info] :
             ccllParser.getBlocks() | std::views::filter(single_block_definition)) {
            auto id = (as<Id>(block_info.block_id) << shift_size);

            generated_blocks.insert(block_name);
            generatedHeader.append(fmt::format("\n{}    {} = {},", extraSpaces, block_name, id));
        }

        generatedHeader.append(fmt::format("\n{}    {} = {},", extraSpaces, "EOI", 0));
        generatedHeader.append(fmt::format("\n{}    {} = {},", extraSpaces, "BAD_TOKEN", 1));

        auto single_rule_definition = [&generated_cases](const auto &rule) {
            return !generated_cases.contains(rule.name);
        };

        for (auto &&rule : ccllParser.getRules() | std::views::filter(single_rule_definition)) {
            auto id = (as<Id>(rule.block_id) << shift_size) | (rule.id);

            generated_cases.insert(rule.name);
            generatedHeader.append(fmt::format("\n{}    {} = {},", extraSpaces, rule.name, id));
        }
    }

    auto StaticGenerator::generateVariable() -> void
    {
        if (includeDirForSrc.empty()) {
            generateLexicalAnalyzer(generatedHeader, "static inline");
            generateRuleNames(generatedHeader, "static inline");
        } else {
            appendToStr(
                generatedHeader,
                fmt::format("extern ccl::lex::LexicalAnalyzer {};\n", variableName));

            appendToStr(
                generatedHeader,
                fmt::format(
                    "extern const ccl::UnorderedMap<ccl::Id, ccl::string_view> ToString{}Token;\n",
                    variableName));
        }
    }

    auto StaticGenerator::generateLexicalAnalyzer(std::string &string, std::string addition_flags)
        -> void
    {
        if (!addition_flags.empty() && ' ' != addition_flags.back()) {
            addition_flags.push_back(' ');
        }

        appendToStr(
            string,
            fmt::format(
                "// NOLINTNEXTLINE\n{0}{1}ccl::lex::LexicalAnalyzer {2}(\n    {0}{3},\n    {0}{{\n",
                extraSpaces, addition_flags, variableName, handler));

        generateRules(string);
        appendToStr(string, "    });\n\n");
    }

    auto StaticGenerator::generateRules(std::string &string) -> void
    {
        for (auto &&rule : ccllParser.getRules()) {
            auto rule_name = fmt::format("{}::{}", enumName, rule.name);
            appendToStr(
                string,
                fmt::format("        {{ {}, R\"( {} )\" }},\n", rule_name, rule.definition));
        }
    }

    auto StaticGenerator::appendToStr(std::string &dest, const std::string &string) const -> void
    {
        if (!extraSpaces.empty()) {
            dest.append(extraSpaces);
        }

        dest.append(string);
    }
}// namespace ccl::lex::gen
