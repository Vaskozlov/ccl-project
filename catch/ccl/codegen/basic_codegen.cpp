#include "ccl/codegen/basic_codegen.hpp"
#include "ccl/debug/debug_file.hpp"

TEST_CASE("BasicCodegen", "[Codegen]")
{
    using namespace ccl::codegen;

    auto generator = BasicCodeGenerator{};

    generator << setStream(1);
    generator << "Test begin" << endl;
    generator << push_scope << "Text in scope\n(4)" << pop_scope << endl;
    generator << setScopeSize(9) << push_scope << "Text in scope\n(9)" << pop_scope << endl;
    generator << "Test end" << endl;

    generator << setStream(0) << "Writing at the begin\n";

    DEBUG_VAR generated_code = generator.getCode();

    REQUIRE(
        generated_code ==
        "Writing at the begin\n"
        "Test begin\n"
        "    Text in scope\n"
        "    (4)\n"
        "             Text in scope\n"
        "             (9)\n"
        "Test end\n");
}
