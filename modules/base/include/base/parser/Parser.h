#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include "GenerateRule.h"

namespace parsertest { namespace parser {

class Parser {
public:
	using DictType = std::unordered_map<std::string, std::unordered_set<std::string>>;

	static constexpr const char * const TERMINAL_EPSILON = "epsilon";
	static constexpr const char * const TERMINAL_EOF = "<EOF>";

private:
	using RuleContainer = std::vector<GenerateRule>;
	using RuleMap = std::unordered_map<std::string, size_t>;

	std::unordered_set<std::string> _terminals;
    std::unordered_set<std::string> _noneTerminals;
	RuleContainer _rules;
	RuleMap _ruleMap;
    std::optional<std::string> _mandatoryStartSymbol;

public:
	Parser();

	///
	void addTermimal(std::string);
	void addNoneTermimal(std::string);
	
	inline void setStartSymbol(const std::string &);
	void addGenerateRule(const std::string &nt, std::vector<std::string> symbols);

	DictType generateFirstSet();
	DictType generateFollowSetWithFristSet(const DictType &firstSet);
    
    ///accessor
    static bool ruleEpsilonable(const GenerateRule &rule);
    
    std::string startSymbol() const;
    bool symbolEpsilonable(const std::string &symbol) const;
    
    inline std::unordered_set<std::string> terminals() const {
        return _terminals;
    }
    
    inline std::unordered_set<std::string> noneTerminals() const {
        return _noneTerminals;
    }
    
    inline RuleContainer generateRuls() const {
        return _rules;
    }
    
    inline bool isTerminal(const std::string &symbol) const {
        return _terminals.count(symbol) > 0;
    }
    
    inline bool isNoneTerminal(const std::string &symbol) const {
        return _noneTerminals.count(symbol) > 0;
    }
    
};

void Parser::setStartSymbol(const std::string &sym) {
    _mandatoryStartSymbol = sym;
}

}}
