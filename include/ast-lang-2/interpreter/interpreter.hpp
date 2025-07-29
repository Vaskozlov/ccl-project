#ifndef AST_LANG_2_INTERPRETER_HPP
#define AST_LANG_2_INTERPRETER_HPP

#include <ast-lang-2/function/functions_holder.hpp>
#include <ast-lang-2/interpreter/stack.hpp>
#include <ast-lang-2/ts/type_system.hpp>
#include <ccl/parser/rules_reader/parser_builder.hpp>

namespace astlang2::interpreter
{
    class Interpreter
      : public Stack
      , public TypeSystem
      , public function::FunctionsHolder
    {
    private:
        ccl::parser::reader::ParserBuilder &constructor;
        std::back_insert_iterator<std::string> outputBuffer;

    public:
        const SmallId TRUE_VALUE = constructor.getRuleId("\"true\"");
        const SmallId FALSE_VALUE = constructor.getRuleId("\"false\"");
        const SmallId NUMBER = constructor.getRuleId("NUMBER");
        const SmallId FLOAT = constructor.getRuleId("FLOAT");
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
        const SmallId ASSIGNMENT_EXPRESSION = constructor.getRuleId("ASSIGNMENT_EXPRESSION");

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

        const SmallId ASSIGN = constructor.getRuleId("\'=\'");

        const astlang2::Value BOOL_TRUE =
            astlang2::Value{.object = toAstlangObject(true), .type = getBool()};

        const astlang2::Value BOOL_FALSE =
            astlang2::Value{.object = toAstlangObject(false), .type = getBool()};

        explicit Interpreter(
            ccl::parser::reader::ParserBuilder &parser_builder,
            std::back_insert_iterator<std::string>
                buffer_inserter);

        auto callFunction(const std::string &name, const isl::SmallVector<Value, 4> &arguments)
            -> Value;

        [[nodiscard]] auto getOutputBuffer() const -> std::back_insert_iterator<std::string>
        {
            return outputBuffer;
        }
    };
}// namespace astlang2::interpreter

#endif /* AST_LANG_2_INTERPRETER_HPP */
