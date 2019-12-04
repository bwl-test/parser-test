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
            parseCmds();
            break;
        
        case TokenType_EOF:
            fmt::print("parse done!\n");
            break;
            
        default:
            errorOut();
            break;
    }
    
    return;
}

void Parser::parseCmd() {
    auto ret = parseExpression();
    fmt::print("{}\n", ret);
}

double Parser::parseExpression() {
    auto left = parseTerm();
    return parseExpressionPrime(left);
}

double Parser::parseExpressionPrime(double left) {
    auto ret = left;
    switch (_curToken.type) {
        //exp' -> (+|-) exp
        case TokenType_ADD:
            eatToken(_curToken.type);
            ret += parseExpression();
            break;
            
        case TokenType_SUB:
            eatToken(_curToken.type);
            ret -= parseExpression();
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
    
    return ret;
}

double Parser::parseTerm() {
    auto left = parseFactor();
    return parseTermPrime(left);
}

double Parser::parseTermPrime(double left) {
    auto ret = left;
    switch (_curToken.type) {
        case TokenType_MUL:
            eatToken(_curToken.type);
            ret *= parseTerm();
            break;
            
        case TokenType_DIV:
            eatToken(_curToken.type);
            ret /= parseTerm();
            break;
        
        case TokenType_ADD: case TokenType_SUB:
        case TokenType_SIMICOLON: case TokenType_EOL:
        case TokenType_RP:  case TokenType_EOF:
            break;
            
        default:
            errorOut();
            break;
    }
    
    return ret;
}

double Parser::parseFactor() {
    double ret =  0;
    auto const &token = _curToken;
    switch (token.type) {
        case TokenType_NUM:
            ret = token.value;
            loadToken();
            break;
        
        case TokenType_LP:
            eatToken(TokenType_LP);
            ret = parseExpression();
            eatToken(TokenType_RP);
            break;
            
        //factor -> (+|-)term
        case TokenType_ADD:
            eatToken(_curToken.type);
            ret = parseTerm();
            break;
            
        case TokenType_SUB:
            eatToken(_curToken.type);
            ret = -parseTerm();
            break;

        default:
            errorOut();
            break;
    }
    
    return ret;
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
