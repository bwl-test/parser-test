#pragma once

#include "Lexer.h"
#include <string>
#include "Token.h"
#include <lexertl/state_machine.hpp>
#include <lexertl/match_results.hpp>

namespace parsertest { namespace calculator {

class SMLexer : public Lexer {
private:
    lexertl::state_machine _sm;
    lexertl::smatch _results;
    BookingBlock _bbl;

public:
    SMLexer(const std::string &str);
    Token nextToken() override;
	inline const BookingBlock& getBookingBlock() const override { return _bbl;}
    
private:
    void errorOut(const std::string &err) const;
    void lookup();
};


}}
