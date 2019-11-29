#pragma once
#include <cstdio>
#include <string>
#include <unordered_map>

namespace parsertest {

class StringUtil {
public:
    static std::string string_from( char const ch ) { return std::string( 1, ch ); }     // Note: "string{1,ch}" is something else.

    static bool is_printable( char const ch ) { return !!::isprint( static_cast<unsigned char>( ch ) ); }

    inline static std::string escape(char ch);

    inline static std::string escape(std::string const& s);
};

std::string 
StringUtil::escape(char ch) {
    static std::unordered_map<char, std::string> const escapes = {
        { '\a', "\\a" },        //  7, ^G, alert (bell)
        { '\b', "\\b" },        //  8, ^H, backspace
        { '\t', "\\t" },        //  9, ^I, tab
        { '\n', "\\n" },        // 10, ^J, newline / linefeed
        { '\v', "\\v" },        // 11, ^K, vertical tab
        { '\f', "\\f" },        // 12, ^L, formfeed
        { '\r', "\\r" },        // 13, ^M, carriage return
        {   27, "\\e" },        // 27, ^[, escape (NON-STANDARD)
        { '\\', "\\\\" }        // backslash
    };

    auto const it = escapes.find( ch );
    if( it != escapes.end() ) {
        return it->second;
    } else if( is_printable( ch ) ) {
        return string_from( ch );
    } else {
        int const code = static_cast<unsigned char>( ch );
        char buf[] = "\\xDDDD";
        sprintf( buf + 2, "%04X", code );
        return buf;
    }
}

std::string 
StringUtil::escape(const std::string &s) {
    std::string result;
    for(auto ch : s) {
        result += escape( ch );
    }
    return result;
}

}
