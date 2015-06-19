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
        Convertor(std::string filename);

    private:
        bool getOutputFile();
        void getClassName();
        std::string getExt();
        bool initialize();

        void run();

        void initializeNewLoop();
        bool commentTesterOpen();
        void commentTesterClose();
        bool firstCheck();
		bool handleNamespace();
		bool handleStruct();
        bool cutLine();
        void handleParameters(std::string& p);
        bool secondCheck();
        void handlePost();
        void handleCtor();
        void handleDtor();
        void handleVoid();

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
        bool mStruct;
        std::string mStructName;
};

#endif // CONVERTOR_HPP
