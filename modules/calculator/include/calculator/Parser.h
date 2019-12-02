#pragma once

#include "Lexer.h"

namespace parsertest { namespace calculator {

class Parser {
private:
	Lexer &_lexer;

public:
	Parser(Lexer &lexer) : _lexer(lexer) {}
	
public:
    void parseCmds();
    void parseCmdsPrime();
    void parseExpression();
    void parseExpressionPrime();
    void parseTerm();
    void parseTermPrime();
    void parseAtomic();
    void eatToken(TokenType id);
};

}}
