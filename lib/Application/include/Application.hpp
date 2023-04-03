#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class FileHandler {
 public:
    FileHandler(const std::string &fileName) {
        m_file.open(fileName);
        if (!m_file.is_open()) {
            std::cerr << "Error: failed to open file with name " << fileName
                      << std::endl;
            throw std::runtime_error("Failed to open file");
        }
    }

    ~FileHandler() {
        if (m_file.is_open()) {
            m_file.close();
        }
    }

    void getLine(std::string &line) { std::getline(m_file, line); }

 private:
    std::ifstream m_file;
};

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

    int readArgs(int argc, char **argv, std::string *const fileNameArtist,
                 std::string *const fileNameGender,
                 std::string *const artistName);
    void fillMapGenderByID(
        std::istream &data,
        std::unordered_map<size_t, std::string> *const genderByID);
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
