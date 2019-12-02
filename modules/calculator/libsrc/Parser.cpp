#include <calculator/Parser.h>
#include <fmt/format.h>

namespace parsertest { namespace calculator {

void Parser::parseCmds() {
    parseCmd();
    switch (_curToken.type) {
        case TokenType_SIMICOLON: case TokenType_EOL:
            eatToken(_curToken.type);
            fmt::print("\n");
            parseCmdsPrime();
            break;
        
        case TokenType_EOF:
            fmt::print("\nparse done!\n");
            break;
            
        default:
            errorOut();
            break;
    }
}

void Parser::parseCmdsPrime() {
    parseCmds();
    return;
}

void Parser::parseCmd() {
    parseExpression();
}

void Parser::parseExpression() {
    switch (_curToken.type) {
        //positive or negtive sign;
        case TokenType_ADD: case TokenType_SUB:
            eatToken(_curToken.type);
            parseAtomic();
            break;
            
        case TokenType_NUM:
            parseTerm();
            break;
        
        case TokenType_LP:
            eatToken(TokenType_LP);
            parseExpression();
            eatToken(TokenType_RP);
            break;
            
        default:
            errorOut();
            break;
    }
    
    switch (_curToken.type) {
        case TokenType_MUL: case TokenType_DIV:
            eatToken(_curToken.type);
            parseTerm();
            break;
        
        case TokenType_ADD: case TokenType_SUB:
            eatToken(_curToken.type);
            parseExpressionPrime();
            break;
            
        case TokenType_SIMICOLON: case TokenType_EOL: case TokenType_RP: case TokenType_EOF:
            break;
            
        default:
            errorOut();
            break;
    }
}

void Parser::parseExpressionPrime() {
    parseExpression();
    return;
}

void Parser::parseTerm() {
    parseAtomic();
    switch (_curToken.type) {
        case TokenType_MUL: case TokenType_DIV:
            eatToken(_curToken.type);
            parseTermPrime();
            break;
        
        case TokenType_ADD: case TokenType_SUB:
        case TokenType_SIMICOLON: case TokenType_EOL:
        case TokenType_RP:  case TokenType_EOF:
            break;
            
        default:
            errorOut();
            break;
    }
}

void Parser::parseTermPrime() {
    parseTerm();
    return;
}

void Parser::parseAtomic() {
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
