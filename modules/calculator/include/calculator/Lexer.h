#pragma once

#include "Token.h"

namespace parsertest { namespace calculator {

class Lexer {
public:
    virtual Token nextToken() = 0;
};

}}
