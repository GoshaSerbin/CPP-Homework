#include <iostream>
#include <fstream>
#include <unordered_map>
#include "utils.hpp"

int main(int argc, char* argv[]){
    std::string fileNameArtist = "", fileNameGender = "", artistName = "";
    if(!readInput(argc, argv, fileNameArtist, fileNameGender, artistName))
        return 0;

    std::unordered_map<size_t, std::string> genderByID;
    if (!readGenders(fileNameGender, genderByID))
        return 0;
    
    std::ifstream fileArtist(fileNameArtist);
    if (!fileArtist.is_open()){
        std::cout << "Can not open artist file!" << std::endl;
        return 0;
    } 

    const size_t namePosition = 2; // номер столбца имя исполнителя
    std::string line = "";   
    size_t artistsCounter = 0;
    while (getline(fileArtist, line)){
        std::string currentName = getField(namePosition, line);
        if (currentName == artistName){
            ++artistsCounter;
            if(!printArtist(line, genderByID))
                return 0;
        }
    }
    std::cout << "Total found " << artistsCounter << " artists." << std::endl;
    fileArtist.close();
    return 0;
}