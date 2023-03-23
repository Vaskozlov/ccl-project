#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <cxxopts.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

auto main(int argc, char *argv[]) -> int
{
    auto source_file = std::string{};
    auto output_file = std::string{};
    auto options = cxxopts::Options("ccll", "Lexical analyzer generator for ccl");

    options.add_options()(
        "l,lexical-analyzer-rules",
        "file with rules for lexical analyzer",
        cxxopts::value(source_file))("o,output", "output header name", cxxopts::value(output_file))(
        "h,help", "produce help message");

    options.parse_positional({"l", "o"});

    auto result = options.parse(argc, argv);

    if (result.count("help") == 1) {
        fmt::print("{}\n", options.help());
        return 1;
    }

    if (output_file.empty()) {
        fmt::print(
            "File with rules for lexical analyzer was not specified.\n"
            "Type --help to see how to use ccll\n");
        return 1;
    }

    if (!std::filesystem::exists(source_file)) {
        fmt::print("Source file {} does not exist\n", source_file);
    }

    auto generated_header = ccl::lex::AnalyzerGenerator::generateStaticVersion(source_file);

    auto file_stream = std::fstream{};
    file_stream.open(output_file, std::ios::out);

    if (!file_stream.is_open()) {
        fmt::print("Error: cannot open file {}\n", output_file);
        return 1;
    }

    file_stream << generated_header;
    file_stream.close();

    return 0;
}
