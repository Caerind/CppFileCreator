#include "Convertor.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
    	Convertor o(argv[1]);
    }
    else
    {
    	std::cout << "CppCreator : Need a filename as argument" << std::endl;
    }
    
    return 0;
}
