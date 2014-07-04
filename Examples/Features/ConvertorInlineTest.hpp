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

        inline void getInputFile();
        inline void getOutputFile();
        inline void getClassName();
        inline void initialize();

        void run();

        void initializeNewLoop();
        inline bool commentTester();
        bool firstCheck();
		bool handleNamespace();
        bool cutLine();
        inline bool secondCheck();
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
