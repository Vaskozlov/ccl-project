#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <chrono>
#include <cxxopts.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std::chrono_literals;

auto main(int argc, char *argv[]) -> int
{
    auto source_file = std::filesystem::path{};
    auto output_file = std::filesystem::path{};
    auto options = cxxopts::Options("ccll", "Lexical analyzer generator for ccl");

    options.add_options()("h,help", "produce help message")(
        "l,lexical-analyzer-rules",
        "file with rules for lexical analyzer",
        cxxopts::value(source_file))("o,output", "output header name", cxxopts::value(output_file))(
        "t,time", "measure lexical analyzer creation time");

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
        fmt::print("Source file {} does not exist\n", source_file.string());
    }

    auto print_generation_time = result.count("time") != 0;

    auto begin = std::chrono::high_resolution_clock::now();
    auto generated_header = ccl::lex::AnalyzerGenerator::generateStaticVersion(source_file);

    auto file_stream = std::fstream{};
    file_stream.open(output_file, std::ios::out);

    if (!file_stream.is_open()) {
        fmt::print("Error: cannot open file {}\n", output_file.string());
        return 1;
    }

    file_stream << generated_header;
    file_stream.close();

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = end - begin;

    if (print_generation_time) {
        fmt::print("{} us\n", elapsed / 1us);
    }

    return 0;
}
