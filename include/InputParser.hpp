#pragma once

#include <filesystem>
#include <iostream>

enum class ReturnValues { dataReceived, dataNotReceived };

struct InputData {
 public:
    std::filesystem::path artistFilePath {};
    std::filesystem::path genderFilePath {};
    std::filesystem::path artistName {};
};

class InputParser {
 public:
    static ReturnValues readArgs(int argc, char** argv, InputData& data, std::ostream& out);
};
