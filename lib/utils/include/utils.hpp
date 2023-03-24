#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>

class Utils
{
public:
    static int run(int argc, char *argv[]);
    static bool readArgs(int argc, char *argv[],
                         std::string &fileNameArtist,
                         std::string &fileNameGender,
                         std::string &artistName);
    static void fillMapGenderByID(std::istream &data,
                                  std::unordered_map<size_t, std::string> &genderByID);
    static void findArtists(std::istream &data,
                            const std::string &artistName,
                            const std::unordered_map<size_t, std::string> &genderByID,
                            std::ostream &out);
    static std::string getField(size_t fieldPosition,
                                const std::string &line);
    static void printArtist(const std::string &line,
                            const std::unordered_map<size_t, std::string> &genderByID,
                            std::ostream &out);
};
