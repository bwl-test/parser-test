#pragma once

namespace parsertest { namespace caculator {

enum TokenType {
    TokenType_EOF,
    TokenType_EOL,
    TokenType_ADD,
    TokenType_SUB,
    TokenType_MUL,
    TokenType_DIV,
    TokenType_LP,
    TokenType_RP,
    TokenType_SIMICOLON,
    TokenType_NUM,
    TokenType_IGNORE,
};

struct Token {
    TokenType type;
    double value;   //目前只支持dobule型数字
    
    int line{1};
    int column{1};
};

}}
