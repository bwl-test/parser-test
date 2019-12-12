#include <base/parser/Parser.h>
#include <catch2/catch.hpp>
#include <iostream>
#include <fmt/format.h>
#include <base/Util.h>
#include <unordered_set>

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
    
    parser.addGenerateRule("expr", {"term", "expr'"});
    parser.addGenerateRule("expr'", {});
    parser.addGenerateRule("expr'", {"+", "expr"});
    parser.addGenerateRule("expr'", {"-", "expr"});
    
    parser.addGenerateRule("term", {"factor", "term'"});
    parser.addGenerateRule("term'", {});
    parser.addGenerateRule("term'", {"*", "term"});
    parser.addGenerateRule("term'", {"/", "term"});
    
    parser.addGenerateRule("factor", {"NUM"});
    parser.addGenerateRule("factor", {"(", "exprlist", ")"});
//    parser.addGenerateRule("factor", {"+", "factor"});
//    parser.addGenerateRule("factor", {"-", "factor"});
    REQUIRE(parser.isNoneTerminal("expr"));
    REQUIRE(parser.isNoneTerminal("expr'"));
    REQUIRE(parser.isNoneTerminal("term"));
    REQUIRE(parser.isNoneTerminal("term'"));
    REQUIRE(parser.isNoneTerminal("factor"));
    
    auto const &firset = parser.generateFirstSet();
    for (auto const &pair : firset) {
        fmt::print("{}:", pair.first);
        for (auto const &symbol : pair.second) {
            fmt::print(" {}", parsertest::StringUtil::escape(symbol));
        }
        fmt::print("\n");
    }
    
    fmt::print("follow set: \n ========================\n");
    
    auto const &followset = parser.generateFollowSetWithFristSet(firset);
    for (auto const &pair : followset) {
        fmt::print("{}:", pair.first);
        for (auto const &symbol : pair.second) {
            fmt::print(" {}", parsertest::StringUtil::escape(symbol));
        }
        fmt::print("\n");
    }
}

//rbegin().base() == end()
TEST_CASE("vector reverse-iterator test", "parsebase") {
    std::vector<int> vec{1,2,3,4,5};
    std::cout << *vec.rbegin() << "\n";
}

TEST_CASE("set merge test", "parsebase") {
    std::unordered_set<int> seta{1,2,3};
    std::unordered_set<int> setb{3,4,5};
    
    REQUIRE(seta.size() == 3);
    REQUIRE(setb.size() == 3);
    
    seta.merge(setb);
    
    fmt::print("seta size: {}, setb size: {}\n", seta.size(), setb.size());
    
    REQUIRE(seta.size() == 5);
    REQUIRE(setb.size() == 1);
    REQUIRE(*setb.begin() == 3);
}
