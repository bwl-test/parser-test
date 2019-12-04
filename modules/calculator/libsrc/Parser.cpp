#include <calculator/Parser.h>
#include <fmt/format.h>

namespace parsertest { namespace calculator {

void Parser::parseCmds() {
    if (_curToken.type == TokenType_EOF) {
        fmt::print("empty file!\n");
        return;
    }
    
    parseCmd();
    parseCmdsPrime();
}

void Parser::parseCmdsPrime() {
    switch (_curToken.type) {
        case TokenType_SIMICOLON: case TokenType_EOL:
            eatToken(_curToken.type);
            fmt::print("\n");
            parseCmds();
            break;
        
        case TokenType_EOF:
            fmt::print("\nparse done!\n");
            break;
            
        default:
            errorOut();
            break;
    }
    
    return;
}

void Parser::parseCmd() {
    parseExpression();
}

void Parser::parseExpression() {
    parseTerm();
    parseExpressionPrime();
}

void Parser::parseExpressionPrime() {
    switch (_curToken.type) {
        //exp' -> (+|-) exp
        case TokenType_ADD: case TokenType_SUB:
            eatToken(_curToken.type);
            parseExpression();
            break;
            
        //exp' -> epsilon
        case TokenType_MUL: case TokenType_DIV:
        case TokenType_SIMICOLON: case TokenType_EOL:
        case TokenType_RP: case TokenType_EOF:
            break;
        
        default:
            errorOut();
            break;
    }
    
    return;
}

void Parser::parseTerm() {
    parseFactor();
    parseTermPrime();
}

void Parser::parseTermPrime() {
    switch (_curToken.type) {
        case TokenType_MUL: case TokenType_DIV:
            eatToken(_curToken.type);
            parseTerm();
            break;
        
        case TokenType_ADD: case TokenType_SUB:
        case TokenType_SIMICOLON: case TokenType_EOL:
        case TokenType_RP:  case TokenType_EOF:
            break;
            
        default:
            errorOut();
            break;
    }
    
    return;
}

void Parser::parseFactor() {
    auto const &token = _curToken;
    switch (token.type) {
        case TokenType_NUM:
            fmt::print(" {} ", token.value);
            loadToken();
            break;
        
        case TokenType_LP:
            eatToken(TokenType_LP);
            parseExpression();
            eatToken(TokenType_RP);
            break;
            
        //factor -> (+|-)term
        case TokenType_ADD: case TokenType_SUB:
            eatToken(_curToken.type);
            parseTerm();
            break;

        default:
            errorOut();
            break;
    }
}

void Parser::eatToken(TokenType toketype) {
    if (toketype != _curToken.type) {
        errorOut();
    }
    loadToken();
}

void Parser::errorOut() const {
    auto const &bbl = _lexer.getBookingBlock();
    auto errdetail = fmt::format("{0} at line {1} column {2}: \"{3}\"", "unexpected symbol", bbl.line, bbl.column-1, bbl.tokenString());
    throw std::runtime_error{errdetail};
}

}}
