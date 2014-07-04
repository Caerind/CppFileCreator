#ifndef CONVERTOR_HPP
#define CONVERTOR_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <vector>

class Convertor
{
    public:
        Convertor();

    private:

        void getInputFile(std::string a = "");
        void getOutputFile(int b = 0);
        void getClassName(float c, float d = 0.5);
        void initialize(float e = 0.5, float f = 0);

        void run();

        void initializeNewLoop();
        bool commentTester();
        bool firstCheck();
		bool handleNamespace();
        bool cutLine();
        void handleParameters(std::string& p);
        bool secondCheck();
        void handleStatic();
        void handleCtor();
        void handleDtor();
        void handleVoid();
        void echoType();

        void write();


        void stop();



        enum Type
        {
            None = 0,
            Ctor = 1,
            Dtor = 2,
            Void = 3,
            Return = 4,
        };



    private:
        std::string mInputFile;
        std::string mOutputFile;
        std::string mClassName;
        std::ifstream mInput;
        std::ofstream mOutput;
        std::string mTempLine;
        std::vector<std::string> mWords;
		std::vector<std::string> mNamespaces;
        size_t mLineCount;
        Type mType;
        bool mCommented;
};

#endif // CONVERTOR_HPP
