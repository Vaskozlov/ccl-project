export module ccl.lexer.analyzer_generator:static_generator;

export import ccl.codegen;
export import ccl.lexer.analyzer;
export import :ccll_parser;

export namespace ccl::lexer::generator
{
    class StaticGenerator
    {
    private:
        constexpr static auto ShiftSize = sizeof(std::size_t) * 8 / 2;

        constexpr static auto ImportedLibraries = []() {
            std::array<isl::string_view, 2> result = {"isl", "ccl.lexer.analyzer"};

            std::ranges::sort(result);
            return result;
        }();

        constexpr static auto BuiltinRules =
            isl::StaticFlatmap<isl::string_view, Id, ReservedTokenMaxValue + 1>{
                {"EOI", 0},
                {"BAD_TOKEN", 1},
                {"CUT", 2}};


        Tokenizer &tokenizer;
        parser::CcllParser ccllParser;
        std::string includeDirForSrc{};
        std::string handler{"ccl::handler::Cmd::instance()"};
        std::string variableName{"set_me"};
        std::string filename{"set_me"};
        std::string nameSpace{};
        std::string enumName{};
        std::string generatedHeader{};
        std::string extraSpaces{};
        codegen::BasicCodeGenerator codeGenerator{};

    public:
        explicit StaticGenerator(Tokenizer &input_tokenizer)
          : tokenizer{input_tokenizer}
          , ccllParser{tokenizer}
        {
            generate();
        }

        [[nodiscard]] auto getCode() const noexcept -> std::string
        {
            return codeGenerator.getCode();
        }

    private:
        auto loadDirectives() -> void
        {
            for (const auto &[name, value] : ccllParser.getDirectives()) {
                const auto name_str = name.getRepr();
                auto value_str = isl::as<std::string>(value);

                if ("VAR_NAME" == name_str) {
                    variableName = std::move(value_str);
                } else if ("HANDLER" == name_str) {
                    handler = std::move(value_str);
                } else if ("NAMESPACE" == name_str) {
                    nameSpace = std::move(value_str);
                } else {
                    auto excpetion = text::TextIteratorException(
                        text::ExceptionCriticality::CRITICAL, text::AnalysisStage::PARSING,
                        name.getLocation(), name_str.size(), name.getInlineRepr(),
                        "unrecognizable directive");
                    tokenizer.getHandler().handle(excpetion);
                }
            }

            if (enumName.empty()) {
                enumName = std::format("{}Token", variableName);
            }
        }

        auto generate() -> void
        {
            ccllParser.parse();

            loadDirectives();

            generateFileDefinition();
            generateNamespaceBegin();

            generateEnum();
            generateVariable();

            generateNamespaceEnd();
        }


        auto generateFileDefinition() -> void
        {
            for (const isl::string_view &header_name : ImportedLibraries) {
                codeGenerator << "export import " << header_name << ';' << codegen::endl;
            }

            codeGenerator << codegen::endl;
        }

        auto generateNamespaceBegin() -> void
        {
            if (nameSpace.empty()) {
                return;
            }

            codeGenerator << "export namespace " << nameSpace << codegen::endl
                          << '{' << codegen::endl
                          << codegen::push_scope;
        }

        auto generateNamespaceEnd() -> void
        {
            if (nameSpace.empty()) {
                return;
            }

            codeGenerator << codegen::pop_scope << codegen::endl
                          << "}// namespace " << nameSpace << codegen::endl;
        }

        auto generateRuleNames() -> void
        {
            auto generated_rules = std::set<isl::string_view>{};

            auto output_rule = [this](isl::string_view value) {
                codeGenerator << codegen::endl << '{' << enumName << "::" << value;
                codeGenerator << ", " << '\"' << value << "\"},";
            };

            auto rule_defined = [&generated_rules](const parser::CcllParser::Rule &rule) {
                return generated_rules.contains(rule.name);
            };

            const std::vector<parser::CcllParser::Rule> &rules = ccllParser.getRules();

            codeGenerator << std::format(
                "inline constexpr isl::StaticFlatmap<ccl::Id, isl::string_view, {}> "
                "ToString{}Token\n",
                BuiltinRules.size() + rules.size(), variableName);
            codeGenerator << '{' << codegen::push_scope;

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

            codeGenerator << codegen::pop_scope << codegen::endl << "};";

            if (nameSpace.empty()) {
                codeGenerator << codegen::endl;
            }
        }

        auto generateEnum() -> void
        {
            codeGenerator << "enum " << enumName << " : ccl::Id {";
            codeGenerator << codegen::push_scope;

            auto enum_definition = isl::Raii{[this]() {
                codeGenerator << codegen::pop_scope << codegen::endl;
                codeGenerator << "};" << codegen::endl << codegen::endl;
            }};

            generateEnumCases();
        }

        auto generateEnumCases() -> void
        {
            auto generated_blocks = std::set<isl::string_view>{};
            auto generated_enum_cases = std::set<isl::string_view>{};
            const auto &blocks = ccllParser.getBlocks();
            const auto &rules = ccllParser.getRules();

            auto output_enum_case = [this](isl::string_view name, Id id) {
                codeGenerator << codegen::endl << name << " = " << std::to_string(id) << ',';
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

        auto generateVariable() -> void
        {
            generateLexicalAnalyzer();
            generateRuleNames();
        }

        auto generateLexicalAnalyzer() -> void
        {
            codeGenerator << "// NOLINTNEXTLINE" << codegen::endl;
            codeGenerator << "inline auto " << variableName << " = ccl::lexer::LexicalAnalyzer{";
            codeGenerator << codegen::push_scope << codegen::endl;
            codeGenerator << handler << ',' << codegen::endl;
            codeGenerator << '{' << codegen::push_scope;

            auto generator_definition = isl::Raii{[this]() {
                codeGenerator << codegen::pop_scope << codegen::endl;
                codeGenerator << '}';
                codeGenerator << codegen::pop_scope << codegen::endl;
                codeGenerator << "};" << codegen::endl << codegen::endl;
            }};

            generateRules();
        }

        auto generateRules() -> void
        {
            const std::vector<parser::CcllParser::Rule> &rules = ccllParser.getRules();

            for (const parser::CcllParser::Rule &rule : rules) {
                codeGenerator << codegen::endl << '{';
                codeGenerator << enumName << "::" << rule.name << ", ";
                codeGenerator << "R\"( ";
                codeGenerator << rule.definition;
                codeGenerator << " )\"";
                codeGenerator << "},";
            }
        }
    };
}// namespace ccl::lexer::generator
