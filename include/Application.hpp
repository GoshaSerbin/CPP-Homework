#pragma once

#include <iostream>

class Application {
 public:
    static void run(int argc, char** argv, std::ostream& out = std::cout);
};
