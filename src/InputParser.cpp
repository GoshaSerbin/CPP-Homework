// Copyright 2023 Gosha Serbin

#include "InputParser.hpp"

#include <string>
#include <unordered_map>

ReturnValues InputParser::readArgs(int argc, char** argv, InputData& data, std::ostream& out) {
    std::unordered_map<std::string, std::string> args;
    for (int i = 1; i < argc; i += 2) {
        args[argv[i]] = argv[i + 1];
    }
    if (args.count("--artist_file_path") == 0 || args.count("--gender_file_path") == 0 ||
        args.count("--artist_name") == 0) {
        out << "Requared options: --artist_file_path, --gender_file_path, --artist_name\n";
        return ReturnValues::dataNotReceived;
    }
    data.artistFilePath = args["--artist_file_path"];
    data.genderFilePath = args["--gender_file_path"];
    data.artistName = args["--artist_name"];
    return ReturnValues::dataReceived;
};