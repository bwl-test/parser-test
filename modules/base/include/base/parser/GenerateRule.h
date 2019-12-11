#pragma once

#include <string>
#include <vector>

namespace parsertest { namespace parser {

class GenerateRule {
private:
	std::string _head;
	std::vector<std::vector<std::string>> _bodies;

public:
	GenerateRule(std::string head) : _head(std::move(head)) {}
	void addBody(std::vector<std::string> symbols);
    
    ///accessor
    inline const std::string& head() const {
        return _head;
    }
    
    inline const std::vector<std::vector<std::string>>& bodies() const {
        return _bodies;
    }
};

}}
