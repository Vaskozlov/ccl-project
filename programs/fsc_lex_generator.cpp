#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <fstream>
#include <iostream>

const char *const ConfigurationFile = "programs/fsc_lex_rule.ccll";
const char *const TargetHeaderFile = "include/fsc/lex/lexical_analyzer.hpp";
const char *const TargetSourceFile = "src/fsc/lex/lexical_analyzer.cpp";

auto main() -> int
{
    auto [generated_header, generated_source] =
        ccl::lex::AnalyzerGenerator::generateStaticVersion(ConfigurationFile);

    auto file_stream = std::fstream{};
    file_stream.open(TargetHeaderFile, std::ios::out);

    if (not file_stream.is_open()) {
        fmt::print("Error: cannot open file {}\n", TargetHeaderFile);
        std::cout.flush();
        return 1;
    }

    file_stream << generated_header;
    file_stream.close();

    file_stream.open(TargetSourceFile, std::ios::out);

    if (not file_stream.is_open()) {
        fmt::print("Error: cannot open file {}\n", TargetSourceFile);
        std::cout.flush();
        return 1;
    }

    file_stream << generated_source;
    file_stream.close();

    return 0;
}
