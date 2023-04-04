// Copyright 2023 Gosha Serbin

#include "Application.hpp"

#include <span>

void Application::run(int argc, char **argv) {
    std::string fileNameArtist {};
    std::string fileNameGender {};
    std::string artistName {};
    if (readArgs(argc, argv, &fileNameArtist, &fileNameGender, &artistName,
                 std::cout) != returnValues::fileNamesReceived) {
        return;
    }
    std::unique_ptr<std::ifstream> fileGenderUPtr(
        new std::ifstream(fileNameGender));
    if (!fileGenderUPtr->is_open()) {
        std::cout << "Can not open gender file!" << std::endl;
        return;
    }
    std::unordered_map<size_t, std::string> genderByID;
    fillMapGenderByID(*fileGenderUPtr, &genderByID);

    std::unique_ptr<std::ifstream> fileArtistUPtr(
        new std::ifstream(fileNameArtist));
    if (!fileArtistUPtr->is_open()) {
        std::cout << "Can not open artist file!" << std::endl;
        return;
    }
    findArtists(*fileArtistUPtr, artistName, genderByID, std::cout);
}

returnValues Application::readArgs(int argc, char **argv,
                                   std::string *const fileNameArtist,
                                   std::string *const fileNameGender,
                                   std::string *const artistName,
                                   std::ostream &out) {
    auto args = std::span(argv, static_cast<size_t>(argc));
    for (int i = 1; i < argc; ++i) {
        if (std::string_view(args[i]) == "--help") {
            out << "Requared options: --artist_file_path --gender_file_path "
                   "--artist_name\n";
            return returnValues::fileNamesNotReceived;
        }
        std::string arg(args[i]);
        size_t pos = arg.find('=');
        std::string opt = arg.substr(0, pos);
        std::string val = arg.substr(pos + 1);
        if (opt == "--artist_file_path") {
            *fileNameArtist = val;
        } else if (opt == "--gender_file_path") {
            *fileNameGender = val;
        } else if (opt == "--artist_name") {
            *artistName = val;
        } else {
            std::cout << "Unknown option ignored.\n";
        }
    }
    return returnValues::fileNamesReceived;
}

void Application::fillMapGenderByID(
    std::istream &data,
    std::unordered_map<size_t, std::string> *const genderByID) {
    std::string line;
    while (getline(data, line)) {
        size_t ID = 0;
        try {
            ID = std::stoi(getField(fieldPosition::GENDER_ID, line));
        } catch (std::invalid_argument const &ex) {
            std::cerr << "Gender file is not correct." << std::endl;
            throw;
        }
        std::string name = getField(fieldPosition::GENDER_NAME, line);
        (*genderByID)[ID] = name;
    }
}

bool Application::AllFieldsAreEmpty(
    const std::string &line,
    const std::vector<fieldPosition> &requiredFieldPositions,
    const std::string &emptySeq) {
    for (auto position : requiredFieldPositions) {
        if (getField(position, line) != emptySeq) {
            return false;
        }
    }
    return true;
};

void Application::findArtists(
    std::istream &data, const std::string &artistName,
    const std::unordered_map<size_t, std::string> &genderByID,
    std::ostream &out) {
    std::string line = "";
    size_t artistsCounter = 0;
    out << "YEAR\t\tMONTH\t\tDAY\t\tGENDER" << std::endl;
    while (getline(data, line)) {
        std::string currentName = getField(fieldPosition::NAME, line);
        if (currentName == artistName) {
            ++artistsCounter;
            printArtist(line, genderByID, std::cout);
        }
    }
    out << "Total found " << artistsCounter << " artists." << std::endl;
}

std::string Application::getField(fieldPosition position,
                                  const std::string &line) {
    size_t currentPos = 0;

    for (size_t i = 0; i < position; ++i) {
        currentPos = line.find('\t', currentPos) + 1;
        if (currentPos == 0) return "";
    }
    size_t fieldLength = line.find_first_of("\t", currentPos) - currentPos;
    return line.substr(currentPos, fieldLength);
}

void Application::printArtist(
    const std::string &line,
    const std::unordered_map<size_t, std::string> &genderByID,
    std::ostream &out) {
    const std::string empty = "\\N";
    const std::string sep = "\t\t";

    std::vector<fieldPosition> requiredFieldPositions = {
        fieldPosition::YEAR, fieldPosition::MONTH, fieldPosition::DAY,
        fieldPosition::GENDER};
    if (AllFieldsAreEmpty(line, requiredFieldPositions, empty)) {
        return;
    }
    for (auto position : requiredFieldPositions) {
        const std::string field = getField(position, line);
        if (field == empty) {
            out << "---" << sep;
            continue;
        };
        switch (position) {
            case fieldPosition::GENDER:
                try {
                    size_t genderID = std::stoi(field);
                    out << genderByID.at(genderID) << sep;

                } catch (std::invalid_argument const &ex) {
                    std::cerr << "Gender file is not correct." << std::endl;
                    throw;
                }
                break;
            default:
                out << field << sep;
        }
    };
    out << std::endl;
}
