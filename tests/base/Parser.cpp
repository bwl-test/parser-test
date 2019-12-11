#include <base/parser/Parser.h>
#include <catch2/catch.hpp>

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
