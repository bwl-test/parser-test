#pragma once

#include <string>

namespace parsertest { namespace calculator {

struct BookingBlock {
public:
    int line{1}; int column{1};
    std::string currTokenStr;

public:
	void updateTokenString(const std::string &);
	std::string tokenString() const;
};

}}
