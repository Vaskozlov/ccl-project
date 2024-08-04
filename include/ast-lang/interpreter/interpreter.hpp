#ifndef CCL_PROJECT_ASTLANG_INTERPRETER_HPP
#define CCL_PROJECT_ASTLANG_INTERPRETER_HPP

#include <ast-lang/interpreter/functions.hpp>
#include <ast-lang/interpreter/stack.hpp>
#include <ccl/parser/rules_reader/rules_constructor.hpp>
#include <isl/raii.hpp>

namespace astlang::interpreter
{
    class Interpreter
    {
    private:
        Functions functions;
        Stack variables;
        ccl::parser::reader::RulesConstructor &constructor;

    public:
        const SmallId NUMBER = constructor.getRuleId("NUMBER");
        const SmallId STRING = constructor.getRuleId("STRING");
        const SmallId IDENTIFIER = constructor.getRuleId("IDENTIFIER");
        const SmallId FUNCTION_CALL = constructor.getRuleId("FUNCTION_CALL");
        const SmallId METHOD_CALL = constructor.getRuleId("METHOD_CALL");

        const SmallId VALUE = constructor.getRuleId("VALUE");
        const SmallId FACTOR = constructor.getRuleId("FACTOR");
        const SmallId TERM = constructor.getRuleId("TERM");
        const SmallId EXPRESSION = constructor.getRuleId("EXPRESSION");
        const SmallId COMPARISON = constructor.getRuleId("COMPARISON");
        const SmallId EQUALITY = constructor.getRuleId("EQUALITY");
        const SmallId BITWISE_AND_EXPRESSION = constructor.getRuleId("BITWISE_AND_EXPRESSION");
        const SmallId BITWISE_XOR_EXPRESSION = constructor.getRuleId("BITWISE_XOR_EXPRESSION");
        const SmallId BITWISE_OR_EXPRESSION = constructor.getRuleId("BITWISE_OR_EXPRESSION");
        const SmallId LOGICAL_AND_EXPRESSION = constructor.getRuleId("LOGICAL_AND_EXPRESSION");
        const SmallId LOGICAL_OR_EXPRESSION = constructor.getRuleId("LOGICAL_OR_EXPRESSION");

        const SmallId PLUS = constructor.getRuleId("\'+\'");
        const SmallId MINUS = constructor.getRuleId("\'-\'");

        const SmallId MULTIPLY = constructor.getRuleId("\'*\'");
        const SmallId DIVIDE = constructor.getRuleId("\'/\'");

        const SmallId LESS = constructor.getRuleId("\'<\'");
        const SmallId LESS_EQUAL = constructor.getRuleId("\'<=\'");
        const SmallId GREATER = constructor.getRuleId("\'>\'");
        const SmallId GREATER_EQUAL = constructor.getRuleId("\'>=\'");

        const SmallId EQUAL = constructor.getRuleId("\'==\'");
        const SmallId NOT_EQUAL = constructor.getRuleId("\'!=\'");

        const SmallId LOGICAL_AND = constructor.getRuleId("\'&&\'");
        const SmallId LOGICAL_OR = constructor.getRuleId("\'||\'");

        const SmallId BITWISE_AND = constructor.getRuleId("\'&\'");
        const SmallId BITWISE_OR = constructor.getRuleId("\'|\'");
        const SmallId BITWISE_XOR = constructor.getRuleId("\'^\'");

        explicit Interpreter(ccl::parser::reader::RulesConstructor &rules_constructor)
          : constructor(rules_constructor)
        {}

        auto addFunction(
            FunctionIdentification identification,
            isl::UniquePtr<FunctionInterface>
                function) -> void
        {
            functions.addFunction(std::move(identification), std::move(function));
        }

        [[nodiscard]] auto
            call(const std::string &name, FunctionCallArguments arguments) -> EvaluationResult
        {
            auto conversion_range = std::views::transform(arguments, [](const auto &argument) {
                return argument.type;
            });

            auto function_identification = FunctionIdentification{
                .name = name,
                .returnType = Type::ERROR,
                .parameters = std::vector<Type>{conversion_range.begin(), conversion_range.end()},
            };

            auto *function = functions.getFunction(function_identification);
            return function->call(*this, std::move(arguments));
        }

        [[nodiscard]] auto read(const std::string &name) -> EvaluationResult &
        {
            return variables.read(name);
        }

        [[nodiscard]] auto globalRead(const std::string &name) -> EvaluationResult &
        {
            return variables.globalRead(name);
        }

        [[nodiscard]] auto getRuleId(isl::string_view rule_name) const -> SmallId
        {
            return constructor.getRuleId(rule_name);
        }

        auto pushHardStackScope() -> void
        {
            variables.pushHardStackScope();
        }

        auto popStackScope() -> void
        {
            variables.popStackScope();
        }

        auto write(const std::string &name, EvaluationResult value) -> void
        {
            variables.write(name, std::move(value));
        }

        auto globalWrite(const std::string &name, EvaluationResult value) -> void
        {
            variables.globalWrite(name, std::move(value));
        }

        auto createVariable(const std::string &name, EvaluationResult value) -> void
        {
            variables.createVariable(name, std::move(value));
        }

        auto createGlobalVariable(const std::string &name, EvaluationResult value) -> void
        {
            variables.createGlobalVariable(name, std::move(value));
        }
    };
}// namespace astlang::interpreter

#endif /* CCL_PROJECT_ASTLANG_INTERPRETER_HPP */
