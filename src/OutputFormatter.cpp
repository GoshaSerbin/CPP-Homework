#include "OutputFormatter.hpp"

bool OutputFormatter::allFieldsAreEmpty(const Artist& artist) const {
    return artist.year == emptyFieldValue && artist.month == emptyFieldValue &&
           artist.day == emptyFieldValue && artist.gender == emptyFieldValue;
};

void OutputFormatter::printArtists(const std::vector<Artist>& artists, std::ostream& out) const {
    std::string line {};
    out << "YEAR\t\tMONTH\t\tDAY\t\tGENDER\n";
    for (const auto& artist : artists) {
        printArtist(artist, out);
    }
}

void OutputFormatter::printArtist(const Artist& artist, std::ostream& out) const {
    const std::string sep("\t\t");
    if (allFieldsAreEmpty(artist)) {
        return;
    } else {
        out << artist.year + sep + artist.month + sep + artist.day + sep + artist.gender << "\n";
    }
}