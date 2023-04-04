// Copyright 2023 Gosha Serbin

#include "Application.hpp"

#include <span>

void Application::run(int argc, char** argv) {
    std::filesystem::path fileNameArtist {};
    std::filesystem::path fileNameGender {};
    std::filesystem::path artistName {};
    if (readArgs(argc, argv, fileNameArtist, fileNameGender, artistName, std::cout) !=
        ReturnValues::fileNamesReceived) {
        return;
    }
    std::unique_ptr<std::ifstream> fileGenderUPtr(new std::ifstream(fileNameGender));
    if (!fileGenderUPtr->is_open()) {
        std::cout << "Can not open gender file!" << std::endl;
        return;
    }
    std::unordered_map<size_t, std::string> genderByID;
    fillMapGenderByID(*fileGenderUPtr, genderByID);

    std::unique_ptr<std::ifstream> fileArtistUPtr(new std::ifstream(fileNameArtist));
    if (!fileArtistUPtr->is_open()) {
        std::cout << "Can not open artist file!" << std::endl;
        return;
    }
    std::vector<std::string> artistsInfo(getSuitableArtistsInfo(*fileArtistUPtr, artistName));
    std::cout << "Total found " << artistsInfo.size() << " artists.\n";
    printArtistsInfo(artistsInfo, genderByID, std::cout);
}

ReturnValues Application::readArgs(int argc, char** argv, std::filesystem::path& fileNameArtist,
                                   std::filesystem::path& fileNameGender, std::filesystem::path& artistName,
                                   std::ostream& out) {
    auto args = std::span(argv, static_cast<size_t>(argc));
    for (int i = 1; i < argc; ++i) {
        if (std::string_view(args[i]) == "--help") {
            out << "Requared options: --artist_file_path --gender_file_path "
                   "--artist_name\n";
            return ReturnValues::fileNamesNotReceived;
        }
        std::string arg(args[i]);
        size_t pos = arg.find('=');
        std::string opt = arg.substr(0, pos);
        std::string val = arg.substr(pos + 1);
        if (opt == "--artist_file_path") {
            fileNameArtist = val;
        } else if (opt == "--gender_file_path") {
            fileNameGender = val;
        } else if (opt == "--artist_name") {
            artistName = val;
        } else {
            std::cout << "Unknown option ignored.\n";
        }
    }
    return ReturnValues::fileNamesReceived;
}

void Application::fillMapGenderByID(std::istream& data, std::unordered_map<size_t, std::string>& genderByID) {
    std::string line;
    while (getline(data, line)) {
        size_t ID = std::stoi(getField(fieldPosition::GENDER_ID, line));
        std::string name = getField(fieldPosition::GENDER_NAME, line);
        genderByID[ID] = name;
    }
}

bool Application::AllFieldsAreEmpty(const std::string& line,
                                    const std::vector<fieldPosition>& requiredFieldPositions,
                                    const std::string& emptySeq) {
    for (auto position : requiredFieldPositions) {
        if (getField(position, line) != emptySeq) {
            return false;
        }
    }
    return true;
};

std::vector<std::string> Application::getSuitableArtistsInfo(std::istream& data,
                                                             const std::string& artistName) {
    std::vector<std::string> info;
    std::string line {};
    while (getline(data, line)) {
        std::string currentName = getField(fieldPosition::NAME, line);
        if (currentName == artistName) {
            info.push_back(line);
        }
    }
    return info;
}

void Application::printArtistsInfo(const std::vector<std::string> artistsInfo,
                                   const std::unordered_map<size_t, std::string>& genderByID,
                                   std::ostream& out) {
    std::string line {};
    out << "YEAR\t\tMONTH\t\tDAY\t\tGENDER\n";
    for (const auto& artistInfo : artistsInfo) {
        printArtistInfo(artistInfo, genderByID, out);
    }
}

std::string Application::getField(fieldPosition position, const std::string& line) {
    size_t currentPos = 0;

    for (size_t i = 0; i < position; ++i) {
        currentPos = line.find('\t', currentPos) + 1;
        if (currentPos == 0) return "";
    }
    size_t fieldLength = line.find_first_of("\t", currentPos) - currentPos;
    return line.substr(currentPos, fieldLength);
}

void Application::printArtistInfo(const std::string& line,
                                  const std::unordered_map<size_t, std::string>& genderByID,
                                  std::ostream& out) {
    const std::string empty("\\N");
    const std::string sep("\t\t");

    std::vector<fieldPosition> requiredFieldPositions = {fieldPosition::YEAR, fieldPosition::MONTH,
                                                         fieldPosition::DAY, fieldPosition::GENDER};
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
            case fieldPosition::GENDER: {
                size_t genderID = std::stoi(field);
                out << genderByID.at(genderID) << sep;
                break;
            }
            default:
                out << field << sep;
        }
    };
    out << '\n';
}
