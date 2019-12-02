#pragma once

#include "Token.h"
#include "BookingBlock.h"
#include <string>

namespace parsertest { namespace calculator {


class Lexer {
public:
    virtual Token nextToken() = 0;
    virtual const BookingBlock& getBookingBlock() const = 0;
};

}}
