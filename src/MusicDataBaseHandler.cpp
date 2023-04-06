#include "MusicDataBaseHandler.hpp"

std::string MusicDataBaseHandler::getField(fieldPosition position, const std::string& line) const {
    size_t start = 0;
    size_t end = line.find(fieldSeparator);
    size_t counter = 0;
    while (end != std::string::npos && counter < position) {
        start = end + 1;
        end = line.find(fieldSeparator, start);
        counter++;
    }
    if (counter == position) {
        return line.substr(start, end - start);
    } else {
        return "";
    }
}

void MusicDataBaseHandler::fillMapGenderByID(std::istream& data) {
    std::string line;
    while (getline(data, line)) {
        const size_t ID = std::stoi(getField(fieldPosition::GENDER_ID, line));
        std::string name = getField(fieldPosition::GENDER_NAME, line);
        genderByID[ID] = name;
    }
}

Artist MusicDataBaseHandler::readArtist(const std::string& line) const {
    Artist artist;
    artist.name = getField(fieldPosition::NAME, line);
    artist.year = getField(fieldPosition::YEAR, line);
    artist.month = getField(fieldPosition::MONTH, line);
    artist.day = getField(fieldPosition::DAY, line);
    artist.gender = getField(fieldPosition::GENDER, line);
    if (artist.gender != emptyFieldValue) {
        artist.gender = genderByID.at(std::stoi(artist.gender));
    }
    return artist;
}

std::vector<Artist> MusicDataBaseHandler::getSuitableArtists(std::istream& data,
                                                             const std::string& artistName) const {
    std::vector<Artist> artists;
    std::string line {};
    while (getline(data, line)) {
        std::string currentName = getField(fieldPosition::NAME, line);
        if (currentName == artistName) {
            artists.push_back(readArtist(line));
        }
    }
    return artists;
}
