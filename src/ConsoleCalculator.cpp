// Copyright 2023 Gosha Serbin

#include "ConsoleCalculator.hpp"
#include "ICalculatable.hpp"
#include "Parser.hpp"

#include <iostream>

void ConsoleCalculator::run(int argc, char** argv, std::ostream& out) {
    if (argc < 2) {
        out << "Missing argument!" << std::endl;
        return;
    }
    std::string expression(argv[1]);
    Parser parser(expression);
    ICalculatableUPtr root = parser.parse();
    out << root->Calculate();
}