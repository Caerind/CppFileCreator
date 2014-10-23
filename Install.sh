#!/bin/bash
sudo apt-get install -y build-essentials
g++ -std=c++11 Source/Convertor.hpp Source/Convertor.cpp Source/main.cpp -o cppfilecreator
sudo cp cppfilecreator /usr/bin/cppfilecreator
