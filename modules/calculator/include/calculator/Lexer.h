#pragma once

#include "Token.h"

namespace parsertest { namespace caculator {

class Lexer {
public:
    virtual Token nextToken() = 0;
};

}}
