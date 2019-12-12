#include <base/parser/Parser.h>
#include <catch2/catch.hpp>
#include <iostream>

using namespace parsertest::parser;

TEST_CASE("add terminal test", "parsebase") {
    Parser parser;
    std::initializer_list<const char *> terminals{
        "NUM", "+", "-", "*", "/", "(", ")", ";", "\n"
    };
    
    for (auto term : terminals) {
        parser.addTermimal(term);
    }
    
    REQUIRE(parser.isTerminal("\n"));
    REQUIRE(!parser.isTerminal("\""));
    REQUIRE(parser.terminals().size() == (terminals.size() + 2));
    
    parser.addGenerateRule("exprlist", {"expr", "exprlist'"});
    parser.addGenerateRule("exprlist'", {});
    parser.addGenerateRule("exprlist'", {";", "exprlist"});
    parser.addGenerateRule("exprlist'", {"\n", "exprlist"});
    REQUIRE(parser.isNoneTerminal("exprlist"));
    REQUIRE(parser.isNoneTerminal("exprlist'"));
}

//rbegin().base() == end()
TEST_CASE("vector reverse-iterator test", "parsebase") {
    std::vector<int> vec{1,2,3,4,5};
    std::cout << *vec.rbegin() << "\n";
}
