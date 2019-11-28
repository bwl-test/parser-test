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
