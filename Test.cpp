#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "OneList.h"
#include <string>
#include <cctype>

static OneList<int> makeIntList(std::initializer_list<int> values)
{
    OneList<int> lst;
    for (int v : values)
        lst.push_back(v);
    return lst;
}

static OneList<char> makeCharList(const std::string& s)
{
    OneList<char> lst;
    for (char c : s)
        lst.push_back(c);
    return lst;
}

static std::string toString(OneList<char>& lst)
{
    std::string res;
    for (auto it = lst.begin(); it != lst.end(); ++it)
        res.push_back(*it);
    return res;
}

static void processString(OneList<char>& s)
{
    bool hasDigit = false;
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        if (std::isdigit(static_cast<unsigned char>(*it)))
        {
            hasDigit = true;
            break;
        }
    }

    char repl = hasDigit ? '_' : '-';
    for (auto it = s.begin(); it != s.end(); ++it)
        if (*it == ' ') *it = repl;
}

TEST_CASE("Constructor creates empty list", "[basic]")
{
    OneList<int> lst;
    REQUIRE(lst.empty());
    REQUIRE(lst.size() == 0);
}

TEST_CASE("push_front and push_back", "[basic]")
{
    OneList<int> lst;

    lst.push_front(2);
    lst.push_front(1);
    lst.push_back(3);

    REQUIRE(lst.size() == 3);
    REQUIRE(lst.front() == 1);
    REQUIRE(lst.back() == 3);
}

TEST_CASE("operator[] access", "[basic]")
{
    auto lst = makeIntList({10, 20, 30});

    REQUIRE(lst[0] == 10);
    REQUIRE(lst[1] == 20);
    REQUIRE(lst[2] == 30);

    REQUIRE_THROWS_AS(lst[-1], std::out_of_range);
    REQUIRE_THROWS_AS(lst[3], std::out_of_range);
}

TEST_CASE("insert works correctly", "[basic]")
{
    OneList<int> lst;
    lst.insert(0, 2);
    lst.insert(0, 1);
    lst.insert(2, 3);
    lst.insert(1, 10);

    REQUIRE(lst.size() == 4);
    REQUIRE(lst[0] == 1);
    REQUIRE(lst[1] == 10);
    REQUIRE(lst[2] == 2);
    REQUIRE(lst[3] == 3);
}

TEST_CASE("pop_front and pop_back", "[basic]")
{
    auto lst = makeIntList({1, 2, 3});

    lst.pop_front(); 
    REQUIRE(lst.front() == 2);

    lst.pop_back(); 
    REQUIRE(lst.back() == 2);
    REQUIRE(lst.size() == 1);
}

TEST_CASE("remove deletes all occurrences", "[basic]")
{
    auto lst = makeIntList({1, 2, 2, 3, 2});

    lst.remove(2);

    REQUIRE(lst.size() == 2);
    REQUIRE(lst[0] == 1);
    REQUIRE(lst[1] == 3);
}

TEST_CASE("clear empties list", "[basic]")
{
    auto lst = makeIntList({1, 2, 3});
    lst.clear();

    REQUIRE(lst.empty());
    REQUIRE(lst.size() == 0);
}

TEST_CASE("Iterator ++ moves forward", "[iterator]")
{
    auto lst = makeIntList({1, 2, 3});

    auto it = lst.begin();
    REQUIRE(*it == 1);
    ++it;
    REQUIRE(*it == 2);
    ++it;
    REQUIRE(*it == 3);
}

TEST_CASE("Iterator -- moves backward from end()", "[iterator]")
{
    auto lst = makeIntList({1, 2, 3});

    auto it = lst.end();
    --it;
    REQUIRE(*it == 3);
    --it;
    REQUIRE(*it == 2);
    --it;
    REQUIRE(*it == 1);
}

TEST_CASE("Exchange sort sorts list", "[sort]")
{
    auto lst = makeIntList({5, 1, 4, 2, 3});
    lst.sort_exchange();

    REQUIRE(lst.size() == 5);
    REQUIRE(lst[0] == 1);
    REQUIRE(lst[1] == 2);
    REQUIRE(lst[2] == 3);
    REQUIRE(lst[3] == 4);
    REQUIRE(lst[4] == 5);
}

TEST_CASE("Selection sort sorts list", "[sort]")
{
    auto lst = makeIntList({9, 7, 5, 3, 1});
    lst.sort_selection();

    REQUIRE(lst.size() == 5);
    REQUIRE(lst[0] == 1);
    REQUIRE(lst[1] == 3);
    REQUIRE(lst[2] == 5);
    REQUIRE(lst[3] == 7);
    REQUIRE(lst[4] == 9);
}

TEST_CASE("Sorting empty and single element list does nothing", "[sort]")
{
    OneList<int> empty;
    REQUIRE_NOTHROW(empty.sort_exchange());
    REQUIRE_NOTHROW(empty.sort_selection());

    OneList<int> one;
    one.push_back(1);
    one.sort_exchange();
    REQUIRE(one[0] == 1);
}

TEST_CASE("String processing with digit", "[task]")
{
    auto s = makeCharList("a 1 b");
    processString(s);
    REQUIRE(toString(s) == "a_1_b");
}

TEST_CASE("String processing without digit", "[task]")
{
    auto s = makeCharList("hello world");
    processString(s);
    REQUIRE(toString(s) == "hello-world");
}

TEST_CASE("String processing edge cases", "[task]")
{
    auto s1 = makeCharList("");
    processString(s1);
    REQUIRE(toString(s1) == "");

    auto s2 = makeCharList("   ");
    processString(s2);
    REQUIRE(toString(s2) == "---");

    auto s3 = makeCharList("0");
    processString(s3);
    REQUIRE(toString(s3) == "0");
}

TEST_CASE("Exceptions on empty list", "[exceptions]")
{
    OneList<int> lst;
    REQUIRE_THROWS_AS(lst.pop_front(), std::out_of_range);
    REQUIRE_THROWS_AS(lst.pop_back(), std::out_of_range);
    REQUIRE_THROWS_AS(lst.front(), std::out_of_range);
    REQUIRE_THROWS_AS(lst.back(), std::out_of_range);
    REQUIRE_THROWS_AS(lst[0], std::out_of_range);
}
