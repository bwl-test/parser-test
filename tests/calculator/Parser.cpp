#include <calculator/SMLexer.h>
#include <calculator/Parser.h>
#include <catch2/catch.hpp>
#include <fmt/format.h>

using namespace parsertest::calculator;

TEST_CASE("parser test", "parser-test") {
    const std::string str = R"(
        
        1 + 2;3 + 4
        3.1415926*20
		-10 + 20
		-(30 - 20 * 40) / 10)";

    SMLexer lexer{str};
	Parser parser{lexer};
	parser.parseCmds();
}

TEST_CASE("empty file parse test", "parser-test") {
    const std::string str = R"(
)";

    SMLexer lexer{str};
    Parser parser{lexer};
    parser.parseCmds();
}
