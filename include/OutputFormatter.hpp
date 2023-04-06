#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Artist.hpp"

class OutputFormatter {
 public:
    [[nodiscard]] bool allFieldsAreEmpty(const Artist& artist) const;
    void printArtists(const std::vector<Artist>& artists, std::ostream& out) const;
    void printArtist(const Artist& artist, std::ostream& out) const;

    OutputFormatter() : emptyFieldValue("\\N"), fieldSeparator("\t\t") {};

    std::string emptyFieldValue;
    std::string fieldSeparator;
};
