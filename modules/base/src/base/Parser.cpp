#include <base/parser/Parser.h>
#include <fmt/format.h>
#include <base/Util.h>

namespace parsertest { namespace parser {

Parser::Parser() {
	_terminals.insert(TERMINAL_EPSILON);
	_terminals.insert(TERMINAL_EOF);
}

void Parser::addTermimal(std::string term) {
    if (_terminals.count(term) > 0) {
        throw std::logic_error{fmt::format("same terminal symbol detected for name: {}", term)};
    }
    
	if (_noneTerminals.count(term) > 0) {
        throw std::logic_error{fmt::format("same none-terminal symbol detected for name: {}", term)};
	}
    
    _terminals.insert(term);
}

void Parser::addNoneTermimal(std::string nterm) {
    if (_terminals.count(nterm) > 0) {
        throw std::logic_error{fmt::format("same terminal symbol detected for name: {}", nterm)};
    }
    
    if (_noneTerminals.count(nterm) > 0) {
        throw std::logic_error{fmt::format("same none-terminal symbol detected for name: {}", nterm)};
    }
    
    _noneTerminals.insert(nterm);
}

void Parser::addGenerateRule(const std::string &nt, std::vector<std::string> symbols) {
    if (_noneTerminals.count(nt) == 0) {
        addNoneTermimal(nt);
    }
    
    if (symbols.empty()) {
        symbols.push_back(TERMINAL_EPSILON);
    }
    
    auto iter = _ruleMap.find(nt);
    if (iter == _ruleMap.end()) {
        auto rule = GenerateRule{nt};
        rule.addBody(symbols);
        _rules.push_back(std::move(rule));
        _ruleMap.insert(std::make_pair(nt, _rules.rbegin().base()));
        return;
    }
    
    auto &rule = *iter->second;
    rule.addBody(symbols);
    return;
}

Parser::DictType Parser::generateFirstSet() {
    bool changed = true;;
    
    DictType dict;
    while (changed) {
        changed = false;
        
        std::for_each(_rules.begin(), _rules.end(), [&](auto const &rule) {
            auto const &head = rule.head();
            
            for (auto const &body : rule.bodies()) {
                auto size = body.size();
                auto oriFirstSize = dict[head].size();
                
                for (auto i=0; i<size; ++i) {
                    auto const &symbol = body[i];
                    
                    if (isTerminal(symbol)) {
                        dict[head].insert(symbol);
                    } else if (isNoneTerminal(symbol)) {
                        dict[head].merge(dict[symbol]);
                    } else {
                        throw std::logic_error{fmt::format("symbol '{}' is not terminal or none-terminal", StringUtil::escape(symbol))};
                    }
                    
                    if (!changed &&  oriFirstSize!=dict[head].size()) {
                        changed = true;
                    }
                    
                    if (!symbolEpsilonable(symbol)) {
                        break;
                    }
                }
            }
        });
    }
    
    return dict;
}


std::string Parser::startSymbol() const {
    if (_mandatoryStartSymbol) {
        return *_mandatoryStartSymbol;
    }
    
    return _rules[0].head();
}

bool Parser::symbolEpsilonable(const std::string &symbol) const {
    if (isTerminal(symbol)) {
        return symbol == TERMINAL_EPSILON;
    }
    
    auto const &iter = _ruleMap.find(symbol);
    if (iter ==_ruleMap.end()) {
        throw std::logic_error{fmt::format("no rules for symbol '{}'", symbol)};
    }
    
    auto const &rule = *iter->second;
    return ruleEpsilonable(rule);
}

bool Parser::ruleEpsilonable(const GenerateRule &rule) {
    for (auto const &body : rule.bodies()) {
        for (auto const &symbol : body) {
            if (symbol == TERMINAL_EPSILON) {
                return true;
            }
        }
    }
    
    return false;
}


}}
