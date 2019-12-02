#pragma once

#include "Lexer.h"
#include <string>
#include "Token.h"
#include <lexertl/state_machine.hpp>
#include <lexertl/match_results.hpp>

namespace parsertest { namespace calculator {

class SMLexer : public Lexer {
private:
    const std::string &_str;
    lexertl::state_machine _sm;
    lexertl::smatch _results;
    int _line{1}; int _column{1};
    std::string _currSubStr;

public:
    SMLexer(const std::string &str);
    Token nextToken() override;
    
private:
    void errorOut(const std::string &err) const;
    void updateBookkeepingInfo();
    void lookup();
};


}}
