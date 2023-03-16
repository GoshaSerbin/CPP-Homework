#pragma once

std::string getField(size_t fieldPosition, const std::string& line);

bool readGenders(const std::string& fileName,\
                 std::unordered_map<size_t, std::string>& genderByID);

bool printArtist(const std::string& line, const std::unordered_map<size_t,\
                 std::string>& genderByID);
                 
bool readInput(int argc, char* argv[], std::string& fileNameArtist,\
               std::string& fileNameGender, std::string& artistName);
