#pragma once

#include "Artist.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class MusicDataBaseHandler {
 public:
    enum fieldPosition {
        GENDER_ID = 0,
        GENDER_NAME = 1,
        NAME = 2,
        YEAR = 4,
        MONTH = 5,
        DAY = 6,
        GENDER = 12
    };
    MusicDataBaseHandler() : emptyFieldValue("\\N"), fieldSeparator("\t") {};

    [[nodiscard]] std::string getField(fieldPosition position, const std::string& line) const;
    [[nodiscard]] std::vector<Artist> getSuitableArtists(std::istream& data,
                                                         const std::string& artistName) const;
    [[nodiscard]] Artist readArtist(const std::string& line) const;
    void fillMapGenderByID(std::istream& data);

    std::unordered_map<size_t, std::string> genderByID;

 private:
    const std::string fieldSeparator;
    const std::string emptyFieldValue;
};
