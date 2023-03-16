#include <iostream>
#include <fstream>
#include <unordered_map>
#include "utils.hpp"

// Возвращает поле столбца fieldPosition строки line
std::string getField(size_t fieldPosition, const std::string& line){
    size_t currentPos = 0;
    // Доходим до столбца fieldPosition
    for(size_t i = 0; i < fieldPosition; ++i) {    
        currentPos = line.find('\t', currentPos) + 1;
    }
    size_t fieldLength = line.find_first_of("\t\n", currentPos) - currentPos;
    return line.substr(currentPos, fieldLength);
}

// Заполняет контейнер genderByID, возвращает 0 если не получилось
bool readGenders(const std::string& fileName,\
                 std::unordered_map<size_t, std::string>& genderByID){
    // Номера столбцов
    const size_t IDPosition = 0;
    const size_t namePosition = 1;
    
    std::ifstream file(fileName);
    if (!file.is_open()){
        std::cout << "Can not open gender file!" << std::endl;
        return 0;
    }

    std::string line;
    while (getline(file, line)){
        size_t ID = 0;      
        try{
            ID = std::stoi(getField(IDPosition, line));
        }
        catch(std::invalid_argument const& ex){
            std::cout << "Gender file is not correct." << std::endl;
            return 0;
        }       
        std::string name = getField(namePosition, line);       
        genderByID[ID] = name;
    }
    return 1;
}

// Выводит данные об исполнителе в строке line, возвращает 0 если не получилось
bool printArtist(const std::string& line, const std::unordered_map<size_t,\
                 std::string>& genderByID){
    // Номера столбцов
    const size_t yearPosition = 4;       // Год рождения
    const size_t monthPosition = 5;      // Месяц рождения
    const size_t dayPosition = 6;        // День рождения
    const size_t genderPosition = 12;    // Пол исполнителя

    const std::string empty = "\\N";

    std::string year = getField(yearPosition, line);
    std::string month = getField(monthPosition, line);
    std::string day = getField(dayPosition, line);
    std::string gender = getField(genderPosition, line);  
    if (year == empty && month == empty && day == empty && gender == empty)
        return 1;

    if (year != empty)
        std::cout << "year: " + year << std::endl;
    if (month != empty)
        std::cout << "month: " + month << std::endl;  
    if (day != empty)
        std::cout << "day: " + day << std::endl; 
    if (gender != empty){
        try{
            size_t genderID = std::stoi(gender);
            std::cout << "gender: " + genderByID.at(genderID) << std::endl; 
        }
        catch(std::invalid_argument const& ex){
            std::cout << "Gender file is not correct." << std::endl;
            return 0;
        }        
    }
    std::cout << std::endl;
    return 1;
}

// Если данные необходимые для работы программы не получены - возвращает 0
bool readInput(int argc, char* argv[], std::string& fileNameArtist,\
               std::string& fileNameGender, std::string& artistName) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == "--help"){
            std::cout << "Requared options:" << std::endl;
            std::cout << "--artist_file_path" << std::endl;
            std::cout << "--gender_file_path" << std::endl;
            std::cout << "--artist_name" << std::endl;
            return 0;
        }
        std::string arg(argv[i]);       
        size_t pos = arg.find('=');
        std::string opt = arg.substr(0, pos);
        std::string val = arg.substr(pos+1);
        if (opt == "--artist_file_path")
            fileNameArtist = val;
        else if (opt == "--gender_file_path")
            fileNameGender = val;
        else if (opt == "--artist_name")
            artistName = val;
        else std::cout << "Uknown option ignored." << std::endl;
    }
    return 1;
}