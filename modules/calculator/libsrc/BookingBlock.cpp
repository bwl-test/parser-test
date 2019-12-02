#include <calculator/BookingBlock.h>
#include <base/Util.h>

namespace parsertest { namespace calculator {

void BookingBlock::updateTokenString(const std::string &str) {
    for (auto ch : str) {
        if (ch == '\n') {
            ++line;
            column = 1;
        } else {
            ++column;
        }
    }
    currTokenStr = str;
}

std::string BookingBlock::tokenString() const {
	if (currTokenStr.empty()) {
		return "<EOF>";
	}
	
	return StringUtil::escape(currTokenStr);
}

}}