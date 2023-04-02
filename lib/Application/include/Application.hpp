#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

class Application {
 public:
    void run(int argc, char **argv);

 private:
    int readArgs(int argc, char **argv, std::string *const fileNameArtist,
                 std::string *const fileNameGender,
                 std::string *const artistName);
    void fillMapGenderByID(
        std::istream &data,
        std::unordered_map<size_t, std::string> *const genderByID);
    void findArtists(std::istream &data, const std::string &artistName,
                     const std::unordered_map<size_t, std::string> &genderByID,
                     std::ostream &out);
    std::string getField(size_t fieldPosition, const std::string &line);
    void printArtist(const std::string &line,
                     const std::unordered_map<size_t, std::string> &genderByID,
                     std::ostream &out);
};
