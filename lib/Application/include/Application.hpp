#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace {
enum class ReturnValues { fileNamesReceived, fileNamesNotReceived };
}

class Application {
 public:
    void run(int argc, char** argv);

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

    [[nodiscard]] ReturnValues parseArgs(int argc, char** argv, std::filesystem::path& fileNameArtist,
                                         std::filesystem::path& fileNameGender,
                                         std::filesystem::path& artistName, std::ostream& out);
    void fillMapGenderByID(std::istream& data, std::unordered_map<size_t, std::string>& genderByID);
    [[nodiscard]] bool allFieldsAreEmpty(const std::string& line,
                                         const std::vector<fieldPosition>& requiredFieldPositions,
                                         const std::string& emptySeq);
    [[nodiscard]] std::string getField(fieldPosition position, const std::string& line);
    [[nodiscard]] std::vector<std::string> getSuitableArtistsInfo(std::istream& data,
                                                                  const std::string& artistName);
    void printArtistsInfo(const std::vector<std::string> artistsInfo,
                          const std::unordered_map<size_t, std::string>& genderByID, std::ostream& out);
    void printArtistInfo(const std::string& line, const std::unordered_map<size_t, std::string>& genderByID,
                         std::ostream& out);
};
