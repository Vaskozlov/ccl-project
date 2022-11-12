#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

const char *const ConfigurationFile = "programs/fsc_lex_rule.ccll";
const char *const TargetHeaderFile = "include/fsc/lex/lexical_analyzer.hpp";
const char *const TargetSourceFile = "src/fsc/lex/lexical_analyzer.cpp";

namespace po = boost::program_options;

auto main(int argc, char *argv[]) -> int
{

    /*
    auto [generated_header, generated_source] =
        ccl::lex::AnalyzerGenerator::generateStaticVersion(ConfigurationFile);

    auto file_stream = std::fstream{};
    file_stream.open(TargetHeaderFile, std::ios::out);

    if (!file_stream.is_open()) {
        fmt::print("Error: cannot open file {}\n", TargetHeaderFile);
        return 1;
    }

    file_stream << generated_header;
    file_stream.close();

    file_stream.open(TargetSourceFile, std::ios::out);

    if (!file_stream.is_open()) {
        fmt::print("Error: cannot open file {}\n", TargetSourceFile);
        return 1;
    }

    file_stream << generated_source;
    file_stream.close();
     */

    return 0;
}
