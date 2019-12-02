#include <calculator/Parser.h>
#include <fmt/format.h>

namespace parsertest { namespace calculator {

void Parser::parseCmds() {
    parseCmd();
    parseCmdsPrime();
    
    if (_unhandledToken) {
        errorOut("unexpected symbol");
    }
}

void Parser::parseCmdsPrime() {
    auto const &token = nextToken();
    
    if (token.type==TokenType_SIMICOLON || token.type==TokenType_EOL || token.type==TokenType_EOF) {
        fmt::print(";\n");
        parseCmds();
    } else {
        setUnhandleToken(token);
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
    auto const &token = nextToken();
    if (token.type!=TokenType_ADD || token.type!=TokenType_SUB) {
        errorOut("unexpected symbol");
    }
    
    if (token.type==TokenType_ADD) {
        fmt::print(" operator + ");
        parseExpression();
        return;
    }
    
    if (token.type==TokenType_SUB) {
        fmt::print(" operator - ");
        parseExpression();
        return;
    }
    
    return;
}

void Parser::parseTerm() {
    parseAtomic();
    parseTermPrime();
}

void Parser::parseTermPrime() {
    auto const &token = nextToken();
    if (token.type==TokenType_MUL) {
        fmt::print(" operator * ");
        parseTerm();
        return;
    } else if (token.type==TokenType_DIV) {
        fmt::print(" operator / ");
        parseTerm();
        return;
    }
    
    errorOut("unexpected symbol");
    return;
}

void Parser::parseAtomic() {
    auto const &token = nextToken();
    switch (token.type) {
        case TokenType_NUM:
            fmt::print("{}", token.value);
            break;
            
        case TokenType_LP:
            fmt::print("(");
            parseExpression();
            break;
            
        case TokenType_RP:
            fmt::print(")");
            break;
            
        case TokenType_SUB:
            fmt::print("-");
            parseAtomic();
            break;
            
        case TokenType_ADD:
            fmt::print("+");
            parseAtomic();
            break;
            
        default:
            errorOut("unexpected symbol");
            break;
    }
    
    nextToken();
}

void Parser::eatToken(const std::function<bool (const Token &)> &predict) {
    auto token = _lexer.nextToken();
    if (!predict(token)) {
        errorOut("unexpected symbol");
    }
}

void Parser::errorOut(const std::string &err) const {
    auto const &bbl = _lexer.getBookingBlock();
    auto errdetail = fmt::format("{0} at line {1} column {2}: \"{3}\"", err, bbl.line, bbl.column-1, bbl.tokenString());
    throw std::runtime_error{errdetail};
}

}}
