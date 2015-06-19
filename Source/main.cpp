#include "Convertor.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "-- ---------- --" << std::endl << "-- CppCreator --" << std::endl << "-- ---------- --" << std::endl << std::endl;
    if(argc == 2)
    {
    	Convertor o(argv[1]);
    }
    else
    {
        std::cout << "Filename (.h/.hpp) : ";
        std::string filepath;
        std::getline(std::cin,filepath);
        Convertor o(filepath);
    }
    return 0;
}
