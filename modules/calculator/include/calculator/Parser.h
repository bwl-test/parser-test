#pragma once

#include "Lexer.h"

namespace parsertest { namespace caculator {

class Parser {
private:
	Lexer &_lexer;

public:
	Parser(Lexer &lexer) : _lexer(lexer) {}
	


};

}}