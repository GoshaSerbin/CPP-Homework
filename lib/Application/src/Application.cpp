// Copyright 2023 Gosha Serbin
#include "Application.hpp"
#include <span>

void Application::run(int argc, char **argv) {
    std::string fileNameArtist {};
    std::string fileNameGender {};
    std::string artistName {};
    if (!readArgs(argc, argv, &fileNameArtist, &fileNameGender, &artistName)) {
        return;
    }

    std::ifstream fileGender(fileNameGender);
    if (!fileGender.is_open()) {
        std::cout << "Can not open gender file!" << std::endl;
        return;
    }
    std::unordered_map<size_t, std::string> genderByID;
    fillMapGenderByID(fileGender, &genderByID);

    std::ifstream fileArtist(fileNameArtist);
    if (!fileArtist.is_open()) {
        std::cout << "Can not open artist file!" << std::endl;
        return;
    }
    findArtists(fileArtist, artistName, genderByID, std::cout);
    fileArtist.close();
}

// Если данные необходимые для работы программы не получены - возвращает 0
int Application::readArgs(int argc, char **argv,
                          std::string *const fileNameArtist,
                          std::string *const fileNameGender,
                          std::string *const artistName) {
    auto args = std::span(argv, static_cast<size_t>(argc));
    for (int i = 1; i < argc; ++i) {
        if (std::string_view(args[i]) == "--help") {
            std::cout << "Requared options:" << std::endl;
            std::cout << "--artist_file_path" << std::endl;
            std::cout << "--gender_file_path" << std::endl;
            std::cout << "--artist_name" << std::endl;
            return 0;
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
            std::cout << "Unknown option ignored." << std::endl;
        }
    }
    return 1;
}

// Заполняет контейнер genderByID
void Application::fillMapGenderByID(
    std::istream &data,
    std::unordered_map<size_t, std::string> *const genderByID) {
    // Номера столбцов
    const size_t IDPosition = 0;
    const size_t namePosition = 1;

    std::string line;
    while (getline(data, line)) {
        size_t ID = 0;
        try {
            ID = std::stoi(getField(IDPosition, line));
        } catch (std::invalid_argument const &ex) {
            std::cerr << "Gender file is not correct." << std::endl;
            throw;
        }
        std::string name = getField(namePosition, line);
        (*genderByID)[ID] = name;
    }
}

// Выводит информацию про нужного артиста в out
void Application::findArtists(
    std::istream &data, const std::string &artistName,
    const std::unordered_map<size_t, std::string> &genderByID,
    std::ostream &out) {
    const size_t namePosition = 2;  // номер столбца имя исполнителя
    std::string line = "";
    size_t artistsCounter = 0;
    while (getline(data, line)) {
        std::string currentName = getField(namePosition, line);
        if (currentName == artistName) {
            ++artistsCounter;
            printArtist(line, genderByID, std::cout);
        }
    }
    out << "Total found " << artistsCounter << " artists." << std::endl;
}

// Возвращает поле столбца fieldPosition строки line
std::string Application::getField(size_t fieldPosition,
                                  const std::string &line) {
    size_t currentPos = 0;
    // Доходим до столбца fieldPosition
    for (size_t i = 0; i < fieldPosition; ++i) {
        currentPos = line.find('\t', currentPos) + 1;
        if (currentPos == 0) return "";
    }
    size_t fieldLength = line.find_first_of("\t", currentPos) - currentPos;
    return line.substr(currentPos, fieldLength);
}

// Выводит данные об исполнителе в строке line
void Application::printArtist(
    const std::string &line,
    const std::unordered_map<size_t, std::string> &genderByID,
    std::ostream &out) {
    // Номера столбцов
    const size_t yearPosition = 4;     // Год рождения
    const size_t monthPosition = 5;    // Месяц рождения
    const size_t dayPosition = 6;      // День рождения
    const size_t genderPosition = 12;  // Пол исполнителя

    const std::string empty = "\\N";

    std::string year = getField(yearPosition, line);
    std::string month = getField(monthPosition, line);
    std::string day = getField(dayPosition, line);
    std::string gender = getField(genderPosition, line);
    if (year == empty && month == empty && day == empty && gender == empty) {
        return;
    }
    if (year != empty) {
        out << "year: " + year << std::endl;
    }
    if (month != empty) {
        out << "month: " + month << std::endl;
    }
    if (day != empty) {
        out << "day: " + day << std::endl;
    }
    if (gender != empty) {
        try {
            size_t genderID = std::stoi(gender);
            out << "gender: " + genderByID.at(genderID) << std::endl;
        } catch (std::invalid_argument const &ex) {
            std::cerr << "Gender file is not correct." << std::endl;
            throw;
        }
    }
    out << std::endl;
}
