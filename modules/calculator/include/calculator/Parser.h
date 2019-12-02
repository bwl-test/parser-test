#pragma once

#include "Lexer.h"
#include <functional>
#include <optional>

namespace parsertest { namespace calculator {

class Parser {
private:
	Lexer &_lexer;
    std::optional<Token> _unhandledToken;

public:
	Parser(Lexer &lexer) : _lexer(lexer) {}
    
    inline Token nextToken() {
        Token retToken;
        if (_unhandledToken) {
            retToken = *_unhandledToken;
            _unhandledToken = {};
        } else {
            retToken = _lexer.nextToken();
        }
        
        return retToken;
    }
    
    void setUnhandleToken(const Token &token) {
        _unhandledToken = token;
    }
	
public:
    void parseCmds();
    void parseCmdsPrime();
    void parseCmd();
    void parseExpression();
    void parseExpressionPrime();
    void parseTerm();
    void parseTermPrime();
    void parseAtomic();
    
    void eatToken(const std::function<bool (const Token &)> &predic);
    
    void errorOut(const std::string &) const;
};

}}
