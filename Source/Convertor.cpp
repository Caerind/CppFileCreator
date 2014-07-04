#include "Convertor.hpp"

Convertor::Convertor()
{
    getInputFile();
    getOutputFile();
    getClassName();
    initialize();
    run();
    stop();
}

void Convertor::getInputFile()
{
    std::cout << "Donnez le lien vers le fichier .h/.hpp a convertir :" << std::endl;
    std::cin >> mInputFile;
}

void Convertor::getOutputFile()
{
    size_t found = mInputFile.rfind(".");
    if (found == std::string::npos)
    {
        std::cerr << "Erreur dans le fichier entre..." << std::endl;
        mOutputFile = "";
    }
    else
        mOutputFile = mInputFile.substr(0, found) + ".cpp";
    std::cout << "Fichier de sortie : " << mOutputFile << std::endl;
}

void Convertor::getClassName()
{
    size_t found = mInputFile.rfind(".");
    if (found != std::string::npos)
        mClassName = mInputFile.substr(0, found);
    else
        mClassName = mInputFile;
    while(mClassName.find("/") != std::string::npos)
        mClassName = mClassName.substr(mClassName.find("/")+1, mClassName.size()-1);
    std::cout << "La convertion sur la classe " << mClassName << " va commencer..." << std::endl;
}

void Convertor::initialize()
{
    mInput.open(mInputFile.c_str());
    mOutput.open(mOutputFile.c_str());

    if (!mInput || !mOutput)
        std::cerr << "Erreur dans l'ouverture d'un des fichiers..." << std::endl;

    //TODO : Faire un include
    mOutput << "#include " << std::endl << std::endl;

    mLineCount = 0;
}

void Convertor::run()
{
    while(std::getline(mInput,mTempLine))
    {
        initializeNewLoop();
        if(firstCheck())
            continue;
		if(handleNamespace())
			continue;
        if(cutLine())
            continue;
        if(secondCheck())
            continue;
        handleStatic();
        handleCtor();
        if(mType == Type::None)
            handleDtor();
        if(mType == Type::None)
            handleVoid();
        if(mType == Type::None)
            mType = Type::Return;
        echoType();

        write();
    }
}

void Convertor::initializeNewLoop()
{
    mLineCount++;
    mWords.clear();
    mType = Type::None;
    std::cout << "Convertion de la ligne : " << mLineCount << " ..." << std::endl;
    while(mTempLine.front() == ' ') mTempLine.erase(0,1);
    while(mTempLine.find("\t") != std::string::npos) mTempLine.erase(0,2);
}

bool Convertor::firstCheck()
{
    if (mTempLine.front() == '#' || mTempLine.front() == '{' || mTempLine.front() == '}' || mTempLine.front() == '/' || mTempLine.front() == '*')
        return true;
    return false;
}

bool Convertor::handleNamespace()
{
	size_t found = mTempLine.find("namespace ");
	if(found != std::string::npos)
    {
		mOutput << mTempLine << std::endl << "{" << std::endl << std::endl;
		mNamespaces.push_back(mTempLine.erase(0,10));
		return true;
	}
	return false;
}

bool Convertor::cutLine()
{
    if(mTempLine.back() == ';')
        mTempLine.pop_back();
    else
        return true;
    size_t found = mTempLine.find("(");
    if(found == std::string::npos)
        return true;
    std::string p = mTempLine.substr(found,mTempLine.size()-1);
    mTempLine = mTempLine.substr(0,found);
    std::stringstream stream(mTempLine);
    std::string temp;
    while(std::getline(stream,temp,' ')) mWords.push_back(temp);
    if(p != "") mWords.push_back(p);
    if(mWords.size() < 2)
        return true;
    return false;
}

bool Convertor::secondCheck()
{
    if (mWords[0] == "typedef" || mWords[0] == "enum" || mWords[0] == "public:" || mWords[0] == "public" || mWords[0] == "protected:"
     || mWords[0] == "protected" || mWords[0] == "private:" || mWords[0] == "private" || mWords[0] == "namespace" || mWords[0] == "class"
     || mWords[0] == "friend")
        return true;
    return false;
}

void Convertor::handleStatic()
{
    if(mWords[1] == "static")
        mWords.erase(mWords.begin());
}

void Convertor::handleCtor()
{
    if(mWords.size() == 2 && mWords[0].find("~") == std::string::npos)
        mType = Type::Ctor;
}

void Convertor::handleDtor()
{
    if(mWords.size() == 2 && mWords[0].find("~") != std::string::npos)
        mType = Type::Dtor;
}

void Convertor::handleVoid()
{
    if(mWords.size() == 3 && mWords[0] == "void")
        mType = Type::Void;
}

void Convertor::echoType()
{
    switch(mType)
    {
        case Type::Ctor:
            std::cout << "Le type est : Ctor" << std::endl;
            break;
        case Type::Dtor:
            std::cout << "Le type est : Dtor" << std::endl;
            break;
        case Type::Void:
            std::cout << "Le type est : Void" << std::endl;
            break;
        case Type::Return:
            std::cout << "Le type est : Return" << std::endl;
            break;
        default:
            std::cout << "Le type est : None" << std::endl;
            break;
    }
}

void Convertor::write()
{
    if(mType != Type::None) mOutput << "////////////////////////////////////////////////////////////" << std::endl;
    if(mType == Type::Ctor || mType == Type::Dtor)
    {
        mOutput << mClassName << "::" << mWords[0] << mWords[1] << std::endl;
        mOutput << "{" << std::endl;
        mOutput << "    " << std::endl;
        mOutput << "}" << std::endl;
    }
    if(mType == Type::Void)
    {
        mOutput << mWords[0] << " " << mClassName << "::" << mWords[1] << mWords[2] << std::endl;
        mOutput << "{" << std::endl;
        mOutput << "    " << std::endl;
        mOutput << "}" << std::endl;
    }
    if(mType == Type::Return)
    {
        std::string p = mWords.back();
        mWords.pop_back();
        std::string f = mWords.back();
        mWords.pop_back();
        for(auto w : mWords)
        {
            mOutput << w << " ";
        }
        mOutput << mClassName << "::" << f << p << std::endl;
        mOutput << "{" << std::endl;
        mOutput << "    return ;" << std::endl;
        mOutput << "}" << std::endl;
    }
	mOutput << std::endl;
}

void Convertor::stop()
{
    for(std::string n : mNamespaces)
		mOutput << "} // namespace " << n << std::endl << std::endl;

	mInput.close();
    mOutput.close();
    std::cout << "Fichiers fermes !" << std::endl;
    std::cout << "Generation terminee !" << std::endl;
}
