#pragma once

#include "Lexer.h"
#include <functional>
#include <optional>

namespace parsertest { namespace calculator {

class Parser {
private:
	Lexer &_lexer;
    Token _curToken;

public:
	Parser(Lexer &lexer) : _lexer(lexer) {
        loadToken();
    }
    
    inline void loadToken() {
        _curToken = _lexer.nextToken();
    }
	
public:
    void parseCmds();
    void parseCmdsPrime();
    void parseCmd();
    double parseExpression();
    double parseExpressionPrime(double);
    double parseTerm();
    double parseTermPrime(double);
    double parseFactor();
    
    void eatToken(TokenType toketype);
    void errorOut() const;
};

}}
