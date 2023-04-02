#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

class Application {
 public:
    static int run(int argc, char **argv);
    static int readArgs(int argc, char **argv,
                        std::string *const fileNameArtist,
                        std::string *const fileNameGender,
                        std::string *const artistName);
    static void fillMapGenderByID(
        std::istream &data,
        std::unordered_map<size_t, std::string> *const genderByID);
    static void findArtists(
        std::istream &data, const std::string &artistName,
        const std::unordered_map<size_t, std::string> &genderByID,
        std::ostream &out);
    static std::string getField(size_t fieldPosition, const std::string &line);
    static void printArtist(
        const std::string &line,
        const std::unordered_map<size_t, std::string> &genderByID,
        std::ostream &out);
};
