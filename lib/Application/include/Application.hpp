#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

enum class returnValues { fileNamesReceived, fileNamesNotReceived };

class Application {
 public:
    void run(int argc, char **argv);

 private:
    enum fieldPosition {
        GENDER_ID = 0,
        GENDER_NAME = 1,
        NAME = 2,
        YEAR = 4,
        MONTH = 5,
        DAY = 6,
        GENDER = 12
    };

    returnValues readArgs(int argc, char **argv,
                          std::filesystem::path &fileNameArtist,
                          std::filesystem::path &fileNameGender,
                          std::filesystem::path &artistName, std::ostream &out);
    void fillMapGenderByID(std::istream &data,
                           std::unordered_map<size_t, std::string> &genderByID);
    void findArtists(std::istream &data, const std::string &artistName,
                     const std::unordered_map<size_t, std::string> &genderByID,
                     std::ostream &out);
    bool AllFieldsAreEmpty(
        const std::string &line,
        const std::vector<fieldPosition> &requiredFieldPositions,
        const std::string &emptySeq);
    std::string getField(fieldPosition position, const std::string &line);
    void printArtist(const std::string &line,
                     const std::unordered_map<size_t, std::string> &genderByID,
                     std::ostream &out);
};
