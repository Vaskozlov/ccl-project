#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <fstream>
#include <iostream>

namespace po = boost::program_options;

std::string SourceFile;
std::string HeaderFile;

auto getOptionDescription() -> po::options_description
{
    po::options_description desc("Allowed options");

    desc.add_options()("help,h", "produce help message");

    desc.add_options()(
        "lexical-analyzer-rules,l", po::value(&SourceFile), "file with rules for lexical analyzer");

    desc.add_options()("output,o", po::value(&HeaderFile), "output header name");

    return desc;
}

auto main(int argc, char *argv[]) -> int
{
    auto desc = getOptionDescription();

    po::positional_options_description po;
    po.add("lexical-analyzer-rules", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(po).run(), vm);
    po::notify(vm);

    if (vm.count("help") == 1) {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("output") == 0) {
        fmt::print(
            "File with rules for lexical analyzer was not specified.\n"
            "Type --help to see how to use ccll\n");
        return 1;
    }

    auto generated_header = ccl::lex::AnalyzerGenerator::generateStaticVersion(SourceFile);

    auto file_stream = std::fstream{};
    file_stream.open(HeaderFile, std::ios::out);

    if (!file_stream.is_open()) {
        fmt::print("Error: cannot open file {}\n", HeaderFile);
        return 1;
    }

    file_stream << generated_header;
    file_stream.close();

    return 0;
}
