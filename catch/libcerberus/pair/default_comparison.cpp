#include <cerberus/debug/debug.hpp>
#include <cerberus/pair.hpp>

using PairT = cerb::Pair<int, int, cerb::PairComparison::DEFAULT>;
template struct cerb::Pair<int, int, cerb::PairComparison::DEFAULT>;

GLOBAL_SCOPE
{
    STATIC_VARIABLE PairT first_pair{ 10, 12 };
    STATIC_VARIABLE PairT second_pair{ 10, 10 };

    static_assert(first_pair != second_pair);
    static_assert(!(first_pair == second_pair));

    static_assert(first_pair > second_pair);
    static_assert(!(first_pair < second_pair));

    static_assert(first_pair >= second_pair);
    static_assert(!(first_pair <= second_pair));
}

GLOBAL_SCOPE
{
    STATIC_VARIABLE PairT first_pair{ 10, 10 };
    STATIC_VARIABLE PairT second_pair{ 12, 10 };

    static_assert(first_pair != second_pair);
    static_assert(!(first_pair == second_pair));

    static_assert(first_pair < second_pair);
    static_assert(!(first_pair > second_pair));

    static_assert(first_pair <= second_pair);
    static_assert(!(first_pair >= second_pair));
}

GLOBAL_SCOPE
{
    STATIC_VARIABLE PairT first_pair{ 10, 10 };
    STATIC_VARIABLE PairT second_pair{ 10, 10 };

    static_assert(first_pair == second_pair);
    static_assert(!(first_pair != second_pair));

    static_assert(!(first_pair < second_pair));
    static_assert(!(first_pair > second_pair));

    static_assert(first_pair <= second_pair);
    static_assert(first_pair >= second_pair);
}
