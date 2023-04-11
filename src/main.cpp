// Copyright 2023 Gosha Serbin

#include "ConsoleCalculator.hpp"
#include <string>
#include <iostream>

int main(int argc, char** argv) {
    ConsoleCalculator calculator;
    calculator.run(argc, argv, std::cout);
    return 0;
}