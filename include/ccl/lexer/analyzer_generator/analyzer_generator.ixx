export module ccl.lexer.analyzer_generator:analyzer_generator;

export import isl;
export import ccll;
export import :static_generator;

export namespace ccl::lexer::generator
{
    class AnalyzerGenerator
    {
    public:
        static auto generateStaticVersion(const std::filesystem::path &path)
            -> std::expected<std::string, isl::io::IOError>
        {
            auto file_content = isl::io::readFile(path);

            return file_content.and_then([&path](auto file_content) {
                return generateStaticVersion(path.string(), file_content);
            });
        }

        static auto generateStaticVersion(std::string_view filename, isl::string_view text)
            -> std::expected<std::string, isl::io::IOError>
        {
            auto tokenizer = ccll::CcllAnalyzer.getTokenizer(text, filename);
            auto static_generator = generator::StaticGenerator{tokenizer};

            return static_generator.getCode();
        }
    };
}// namespace ccl::lexer
