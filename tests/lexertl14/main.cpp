#include <lexertl/generator.hpp>
#include <lexertl/lookup.hpp>
#include <iostream>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

TEST_CASE("normal test", "lexer") {
    lexertl::rules rules;
    lexertl::state_machine sm;

    rules.push("[0-9]+", 1);
    rules.push("[a-z]+", 2);
    lexertl::generator::build(rules, sm);

    std::string input("abc012Ad3e4");
    lexertl::smatch results(input.begin(), input.end());

    // Read ahead
    lexertl::lookup(sm, results);

    while (results.id != 0)
    {
        std::cout << "Id: " << results.id << ", Token: '" <<
            results.str () << "'\n";
        lexertl::lookup(sm, results);
    }
}

TEST_CASE("recursive test", "lexer") {
    enum {eEOI, eComment};
    lexertl::rules rules;
    lexertl::state_machine sm;
    std::string input("/* /* recursive */*/");

    rules.push_state("OPEN");

    rules.push("INITIAL,OPEN", "[/][*]", ">OPEN");
    rules.push("OPEN", ".{+}[\r\n]", ".");
    rules.push("OPEN", "[*][/]", 1, "<");
    lexertl::generator::build(rules, sm);

    lexertl::srmatch results(input.begin(), input.end());

    // Read ahead
    lexertl::lookup(sm, results);

    while (results.id != eEOI && results.id != results.npos())
    {
        switch (results.id)
        {
            case eComment:
                std::cout << results.str() << std::endl;
                break;
            default:
                std::cout << "Error at '" << &*results.first << "'\n";
                break;
        }

        lexertl::lookup(sm, results);
    }
}


