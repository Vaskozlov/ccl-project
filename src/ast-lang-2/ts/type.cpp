#include <ast-lang-2/ts/type.hpp>

namespace astlang2::ts
{
    Type::Type(
        std::string type_name,
        const ccl::Id type_id,
        Type *parent_type,
        const bool is_builtin)
      : name{std::move(type_name)}
      , id{type_id}
      , prototype{parent_type}
      , builtin{is_builtin}
    {}

    auto Type::addField(Field field) -> Field *
    {
        auto [it, inserted] = fields.try_emplace(field.name, field);

        if (!inserted) {
            throw std::runtime_error(fmt::format("Field '{}' already exists", field.name));
        }

        return &it->second;
    }

    auto Type::callMethod(
        interpreter::Interpreter &interpreter,
        const std::string &method_name,
        const isl::SmallVector<Value, 4> &arguments) -> Value
    {
        function::FunctionIdentification function_identification{.name = method_name};

        for (const auto &arg : arguments) {
            function_identification.arguments.emplace_back(arg.type);
        }

        const auto *function = methods.getFunction(function_identification);
        return function->call(interpreter, arguments);
    }

    auto Type::getField(Field field) const -> const Field *
    {
        const auto it = fields.find(field.name);

        if (it == fields.end()) {
            throw std::runtime_error{fmt::format("Field '{}' does not exist", field.name)};
        }

        return &it->second;
    }
}// namespace astlang2::ts