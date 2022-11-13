#include <ccl/lex/analyzer_generator/static_generator.hpp>
#include <ranges>

namespace ccl::lex::gen
{
    auto StaticGenerator::loadDirectives() -> void
    {
        for (const auto &[name, value] : ccll_parser.getDirectives()) {
           if (name == "VAR_NAME") {
                variable_name = value;
            } else if (name == "HANDLER") {
                handler = value;
            } else if (name == "NAMESPACE") {
                name_space = value;
            } else {
                fmt::print("unrecognizable directive: {}\n", name);
            }
        }

        if (enum_name.empty()) {
            enum_name = fmt::format("{}Token", variable_name);
        }
    }

    auto StaticGenerator::generate() -> void
    {
        ccll_parser.parse();

        loadDirectives();

        generateHeaderDefinition();
        generateNamespaceBegin(generated_header);

        generateEnum();
        generateVariable();

        generateNamespaceEnd(generated_header);

        generateSource();
    }

    auto StaticGenerator::generateSource() -> void
    {
        generated_source = fmt::format("#include <{}/{}>\n\n", include_dir_for_src, filename);

        generateNamespaceBegin(generated_source);
        generateLexicalAnalyzer(generated_source);
        generateRuleNames(generated_source);
        generateNamespaceEnd(generated_source);
    }

    auto StaticGenerator::generateHeaderDefinition() -> void
    {
        generated_header =
            "#pragma once\n\n"
            "#include <unordered_map>\n"
            "#include <ccl/handler/cmd_handler.hpp>\n"
            "#include <ccl/lex/lexical_analyzer.hpp>\n\n";
    }

    auto StaticGenerator::generateNamespaceBegin(std::string &string) -> void
    {
        if (not name_space.empty()) {
            extra_spaces = "    ";
            string.append(fmt::format("namespace {}\n{{\n", name_space));
        }
    }

    auto StaticGenerator::generateNamespaceEnd(std::string &string) -> void
    {
        if (not name_space.empty()) {
            extra_spaces.clear();
            string.append(fmt::format("}}// namespace {}\n", name_space));
        }
    }

    auto StaticGenerator::generateRuleNames(std::string &string, std::string addition_flags) -> void
    {
        if (not addition_flags.empty() && addition_flags.back() != ' ') {
            addition_flags.push_back(' ');
        }

        appendToStr(
            string,
            fmt::format(
                "{}const std::unordered_map<size_t, ccl::string_view> ToString{}Token\n    {{\n",
                addition_flags, variable_name));

        std::set<string_view> generated_rules{};

        appendToStr(string, fmt::format("    {{ {0}::{1}, \"{1}\" }},\n", enum_name, "EOI"));
        appendToStr(string, fmt::format("    {{ {0}::{1}, \"{1}\" }},\n", enum_name, "BAD_TOKEN"));

        auto repeat_filter = [&generated_rules](const parser::CcllParser::Rule &rule) {
            return not generated_rules.contains(rule.name);
        };

        for (auto &&rule : ccll_parser.getRules() | std::views::filter(repeat_filter)) {
            generated_rules.insert(rule.name);
            appendToStr(
                string, fmt::format("    {{ {0}::{1}, \"{1}\" }},\n", enum_name, rule.name));
        }

        appendToStr(string, "};\n");
    }

    auto StaticGenerator::generateEnum() -> void
    {
        appendToStr(
            generated_header,
            fmt::format(
                "CCL_PREDEFINED_ENUM(// NOLINTNEXTLINE\n    {0}{1},\n    {0}size_t", extra_spaces,
                enum_name));

        generateEnumCases();

        generated_header.append(");\n\n");
    }

    auto StaticGenerator::generateEnumCases() -> void
    {
        constexpr auto shift_size = 16ZU;

        std::set<string_view> generated_cases{};
        std::set<string_view> generated_blocks{};

        auto single_block_definition = [&generated_blocks](const auto &block) {
            const auto &block_name = block.first;
            return not generated_blocks.contains(block_name);
        };

        for (const auto &[block_name, block_info] :
             ccll_parser.getBlocks() | std::views::filter(single_block_definition)) {
            auto id = (as<size_t>(block_info.block_id) << shift_size);

            generated_blocks.insert(block_name);
            generated_header.append(fmt::format(",\n{}    {} = {}", extra_spaces, block_name, id));
        }

        generated_header.append(fmt::format(",\n{}    {} = {}", extra_spaces, "EOI", 0));
        generated_header.append(fmt::format(",\n{}    {} = {}", extra_spaces, "BAD_TOKEN", 1));

        auto single_rule_definition = [&generated_cases](const auto &rule) {
            return not generated_cases.contains(rule.name);
        };

        for (auto &&rule : ccll_parser.getRules() | std::views::filter(single_rule_definition)) {
            auto id = (as<size_t>(rule.block_id) << shift_size) | (rule.id);

            generated_cases.insert(rule.name);
            generated_header.append(fmt::format(",\n{}    {} = {}", extra_spaces, rule.name, id));
        }
    }

    auto StaticGenerator::generateVariable() -> void
    {
        if (include_dir_for_src.empty()) {
            generateLexicalAnalyzer(generated_header, "static inline");
            generateRuleNames(generated_header, "static inline");
        } else {
            appendToStr(
                generated_header,
                fmt::format("extern ccl::lex::LexicalAnalyzer {};\n", variable_name));

            appendToStr(
                generated_header,
                fmt::format(
                    "extern const std::unordered_map<size_t, ccl::string_view> ToString{}Token;\n",
                    variable_name));
        }
    }

    auto StaticGenerator::generateLexicalAnalyzer(std::string &string, std::string addition_flags)
        -> void
    {
        if (not addition_flags.empty() && addition_flags.back() != ' ') {
            addition_flags.push_back(' ');
        }

        appendToStr(
            string,
            fmt::format(
                "// NOLINTNEXTLINE\n{0}{1}ccl::lex::LexicalAnalyzer {2}(\n    {0}{3},\n    {0}{{\n",
                extra_spaces, addition_flags, variable_name, handler));

        generateRules(string);
        appendToStr(string, "    });\n\n");
    }

    auto StaticGenerator::generateRules(std::string &string) -> void
    {
        for (auto &&rule : ccll_parser.getRules()) {
            auto rule_name = fmt::format("{}::{}", enum_name, rule.name);
            appendToStr(
                string,
                fmt::format("        {{ {}, R\"( {} )\" }},\n", rule_name, rule.definition));
        }
    }

    auto StaticGenerator::appendToStr(std::string &dest, const std::string &string) const -> void
    {
        if (not extra_spaces.empty()) {
            dest.append(extra_spaces);
        }

        dest.append(string);
    }
}// namespace ccl::lex::gen
