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
        _ruleMap.insert(std::make_pair(nt, _rules.size()-1));
        return;
    }
    
    auto &rule = _rules[iter->second];
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
                        dict[head].insert(dict[symbol].begin(), dict[symbol].end());
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

Parser::DictType Parser::generateFollowSetWithFristSet(const DictType &firstSet) {
    bool changed = true;;
    
    DictType dict;
    dict[startSymbol()].insert(TERMINAL_EOF);
    
    //每次集合变更都需要记录并比较,故而抽取成lamda
    auto doSomthingAndMarkChange = [&](const std::string &symbol, const std::function<void()> &func) {
        auto orisize = dict[symbol].size();
        func();
        if (!changed && dict[symbol].size()!=orisize) {
            changed = true;
        }
    };
    
    while (changed) {
        changed = false;
        
        std::for_each(_rules.begin(), _rules.end(), [&](auto const &rule) {
            auto const &head = rule.head();
            auto const &bodies = rule.bodies();
            
            for (auto const &body : bodies) {
                auto size = static_cast<int>(body.size());
                
                //1.head的follow集传播到产生式最后一个'非终端'符号里
                auto const &lastSymbol = body.back();
                if (isNoneTerminal(lastSymbol)) {
                    doSomthingAndMarkChange(lastSymbol, [&]() {
                        dict[lastSymbol].insert(dict[head].begin(), dict[head].end());
                    });
                }
                
                //2.计算'产生式体'中各'非终端'符号的follow集
                for (auto i=size-2; i>=0; --i) {
                    auto const &currsym = body[i];
                    if (isTerminal(currsym)) {
                        continue;
                    }
                    
                    doSomthingAndMarkChange(currsym, [&]() {
                        auto const &nextsym = body[i+1];
                        
                        if (isTerminal(nextsym)) {
                            dict[currsym].insert(nextsym);
                        } else if (isNoneTerminal(nextsym)) {
                            dict[currsym].insert(firstSet.at(nextsym).begin(), firstSet.at(nextsym).end());
                            if (symbolEpsilonable(nextsym)) {
                                dict[currsym].insert(dict[nextsym].begin(), dict[nextsym].end());
                            }
                        } else {
                            throw std::logic_error{fmt::format("symbol '{}' is not terminal or none-terminal", StringUtil::escape(nextsym))};
                        }
                    });
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
    
    return ruleEpsilonable(_rules[iter->second]);
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
