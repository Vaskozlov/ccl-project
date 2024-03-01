export module ccl.lexer.generator:analyzer_generator;

export import isl;

export namespace ccl::lexer
{
    class AnalyzerGenerator
    {
    public:
        static auto generateStaticVersion(const std::filesystem::path &path) -> std::string;
        static auto generateStaticVersion(std::string_view filename, isl::string_view text)
            -> std::string;
    };
}// namespace ccl::lexer

