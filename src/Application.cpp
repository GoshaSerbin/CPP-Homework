// Copyright 2023 Gosha Serbin

#include "Application.hpp"
#include "InputParser.hpp"
#include "MusicDataBaseHandler.hpp"
#include "OutputFormatter.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

void Application::run(int argc, char** argv) {
    InputData data;
    if (InputParser::readArgs(argc, argv, data, std::cout) != ReturnValues::dataReceived) {
        return;
    }

    std::unique_ptr<std::ifstream> fileGenderUPtr(new std::ifstream(data.genderFilePath));
    if (!fileGenderUPtr->is_open()) {
        std::cout << "Can not open gender file!" << std::endl;
        return;
    }
    std::unique_ptr<std::ifstream> fileArtistUPtr(new std::ifstream(data.artistFilePath));
    if (!fileArtistUPtr->is_open()) {
        std::cout << "Can not open artist file!" << std::endl;
        return;
    }

    MusicDataBaseHandler db;
    db.fillMapGenderByID(*fileGenderUPtr);
    std::vector<Artist> artists(db.getSuitableArtists(*fileArtistUPtr, data.artistName));
    std::cout << "Total found " << artists.size() << " artists.\n";

    OutputFormatter output;
    output.printArtists(artists, std::cout);
}
