#include <calculator/Lexer.h>
#include <catch2/catch.hpp>
#include <iostream>

using namespace parsertest::caculator;

TEST_CASE("get token test", "lexter-test") {
    const std::string str = R"(
        1+2;
        3+4
        3.1415926*20
        3.1415927E20 + 10
    )";

    Lexer lexer{str};

    while (true) {
        auto token = lexer.nextToken();
        std::cout << "lexer getToken ret: " <<  token.type << "\n";
        if (token.type == TokenType_NUM) {
            std::cout << "number token: " <<  token.value << "\n";
        } else if (token.type == TokenType_EOF) {
            std::cout << "encounter end of file" << "\n";
            break;
        } else {
            //do nothing
        }
    }
}
