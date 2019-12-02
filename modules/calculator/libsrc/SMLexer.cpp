#include <calculator/SMLexer.h>
#include <lexertl/generator.hpp>
#include <lexertl/lookup.hpp>
#include <iostream>
#include <fmt/format.h>
#include <base/Util.h>

namespace parsertest { namespace calculator {

SMLexer::SMLexer(const std::string &str) : _results(str.begin(), str.end()) {
    lexertl::rules rules;

    rules.push_state("COMMAND");
    
    rules.push("INITIAL", "\\n", TokenType_IGNORE, ".");
    rules.push("COMMAND", "\\n", TokenType_EOL, ".");
    
    rules.push("*", "[\\t ]+", TokenType_IGNORE, ".");
    rules.push("*", "\\d+(\\.(\\d+)?)?((e|E)\\d+)?", TokenType_NUM, "COMMAND");
    rules.push("*", "\\+", TokenType_ADD, "COMMAND");
    rules.push("*", "-", TokenType_SUB, "COMMAND");
    rules.push("*", "\\*", TokenType_MUL, "COMMAND");
    rules.push("*", "\\/", TokenType_DIV, "COMMAND");
    rules.push("*", "\\(", TokenType_LP, "COMMAND");
    rules.push("*", "\\)", TokenType_RP, "COMMAND");
    rules.push("*", ";", TokenType_SIMICOLON, "COMMAND");
    
    lexertl::generator::build(rules, _sm);
}

void SMLexer::errorOut(const std::string &err) const {
    auto errdetail = fmt::format("{0} at line {1} column {2} char: \"{3}\"", err, _bbl.line, _bbl.column-1, _bbl.tokenString());
    throw std::runtime_error{errdetail};
}

Token SMLexer::nextToken() {
    lookup();
    
    while (_results.id == TokenType_IGNORE) {
        lookup();
    }
    
    if (_results.id == lexertl::smatch::npos()) {
        errorOut("unrecognized character");
    }
    
    auto token = Token{static_cast<TokenType>(_results.id), 0};;
    if (_results.id == TokenType_NUM) {
        token.value = atof(_results.str().c_str());
    }

    return token;
}

void SMLexer::lookup() {
    lexertl::lookup(_sm, _results);
    _bbl.updateTokenString(_results.str());
}

}}
