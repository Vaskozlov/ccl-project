#include <ast-lang-2/function/functions_holder.hpp>

namespace astlang2::function {
    auto FunctionsHolder::addFunction(FunctionIdentification identification, std::shared_ptr<Function> function) -> void
    {
        auto has_inserted = false;

        std::tie(std::ignore, has_inserted) =
            functions.try_emplace(std::move(identification), std::move(function));

        if (!has_inserted) {
            throw std::runtime_error("Function already exists");
        }
    }

    auto FunctionsHolder::getFunction(const FunctionIdentification &identification) -> Function *
    {
        const auto it = functions.find(identification);

        if (it == functions.end()) {
            throw std::runtime_error("Function not found");
        }

        return it->second.get();
    }
}