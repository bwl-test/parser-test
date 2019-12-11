#include <base/parser/GenerateRule.h>
#include <stdexcept>

namespace parsertest { namespace parser {

bool operator==(const std::vector<std::string> &vec1, const std::vector<std::string> &vec2) {
	if (vec1.size() != vec2.size()) {
		return false;
	}
	
	for(auto i=0; i<vec1.size(); ++i) {
		if (vec1[i] != vec2[i]) {
			return false;
		}
	}

	return true;
}

void GenerateRule::addBody(std::vector<std::string> symbols) {
	if (symbols.empty()) {
		throw std::logic_error{"empty generate rule detected, please use 'epsilon' instead!!"};
	}
	
	for(auto const &rule : _bodies) {
		if (rule == symbols) {
			throw std::logic_error{"duplicated generate rule detected!!"};
		}
	}
	
	_bodies.push_back(std::move(symbols));
	return;
}

}}
