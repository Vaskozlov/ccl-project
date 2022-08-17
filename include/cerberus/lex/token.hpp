#ifndef CERBERUS_PROJECT_TOKEN_HPP
#define CERBERUS_PROJECT_TOKEN_HPP

#include <cerberus/text/text_iterator.hpp>
#include <string>
#include <vector>

namespace cerb::lex
{
    class Token
    {
    public:
        Token() = default;

        Token(typename u8string_view::iterator begin_, size_t id_)
          : repr{ begin_, static_cast<size_t>(0) }, id{ id_ }
        {}

        Token(const text::TextIterator &text_iterator_, size_t id_)
          : repr{ text_iterator_.getRemaining() }, id{ id_ }
        {}

        [[nodiscard]] auto getId() const -> size_t
        {
            return id;
        }

        [[nodiscard]] explicit operator bool() const
        {
            return getId() != 0;
        }

        [[nodiscard]] auto getRepr() const -> u8string_view
        {
            return repr;
        }

        [[nodiscard]] auto getPrefixes() const -> const std::vector<u8string_view> &
        {
            return prefixes;
        }

        [[nodiscard]] auto getPostfixes() const -> const std::vector<u8string_view> &
        {
            return prefixes;
        }

        auto setEnd(typename u8string_view::iterator end_) -> void
        {
            repr = { repr.begin(), end_ };
        }

        auto addPrefix(u8string_view prefix) -> void
        {
            prefixes.push_back(prefix);
        }

        auto addPostfix(u8string_view postfix) -> void
        {
            postfixes.push_back(postfix);
        }

    private:
        std::vector<u8string_view> prefixes{};
        std::vector<u8string_view> postfixes{};
        u8string_view repr{};
        size_t id{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_TOKEN_HPP */
