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

    mOutput << "#include \"" << mClassName << ".hpp\"" << std::endl << std::endl;

    mLineCount = 0;
	mCommented = false;
	mStruct = false;
}

void Convertor::run()
{
    while(std::getline(mInput,mTempLine))
    {
        initializeNewLoop();

        if(commentTesterOpen())
        {
            commentTesterClose();
            continue;
        }

        if(firstCheck())
            continue;
		if(handleNamespace())
			continue;
        if(handleStruct())
            continue;
        if(cutLine())
            continue;
        if(secondCheck())
            continue;
        handlePost();
        if(mType == Type::None)
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
    while(mTempLine.find("\t") != std::string::npos) mTempLine.erase(0,1);
    while(mTempLine.find("\n") != std::string::npos) mTempLine.erase(0,1);
}

bool Convertor::commentTesterOpen()
{
    if(mTempLine.find("/*") == 1 && !mCommented)
        mCommented = true;
    if(mTempLine.find("*/") == 1 && mCommented)
        mCommented = false;
    return mCommented;
}

void Convertor::commentTesterClose()
{
    if(mTempLine.find("*/") != std::string::npos && mCommented)
        mCommented = false;
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

bool Convertor::handleStruct()
{
	if(mTempLine.find("struct ") != std::string::npos && !mStruct)
    {
		mStruct = true;
		mStructName = mTempLine.erase(0,7);
		return true;
	}
	if(mTempLine.find("};") != std::string::npos && mStruct)
    {
        mStruct = false;
        return true;
    }
	return false;
}


bool Convertor::cutLine()
{
    size_t found = mTempLine.find(";");
    if(found == std::string::npos)
        return true;
    if(mTempLine.find("/*") != std::string::npos)
        mCommented = true;
    mTempLine = mTempLine.substr(0,found+1);
    found = mTempLine.find("(");
    if(found == std::string::npos ||(mTempLine.find(");") == std::string::npos && mTempLine.find(") const;") == std::string::npos))
        return true;
    mTempLine.pop_back();
    std::string p = mTempLine.substr(found,mTempLine.size()-1);
    handleParameters(p);
    mTempLine = mTempLine.substr(0,found);
    std::stringstream stream(mTempLine);
    std::string temp;
    while(std::getline(stream,temp,' ')) mWords.push_back(temp);
    if(p != "") mWords.push_back(p);
    if(mWords.size() < 2)
        return true;
    return false;
}

void Convertor::handleParameters(std::string& p)
{
    if(p == "()")
        return;
    while(p.find("=") != std::string::npos)
    {
        size_t found = p.find("=");
        while(p.at(found) != ',' && p.at(found) != ')')
            p.erase(found,1);
        while(p.find(" ,") != std::string::npos)
            p.erase(p.find(" ,"),1);
        while(p.find(" )") != std::string::npos)
            p.erase(p.find(" )"),1);
    }
}

bool Convertor::secondCheck()
{
    if (mWords[0] == "typedef" || mWords[0] == "enum" || mWords[0] == "public:" || mWords[0] == "public" || mWords[0] == "protected:"
     || mWords[0] == "protected" || mWords[0] == "private:" || mWords[0] == "private" || mWords[0] == "namespace" || mWords[0] == "class"
     || mWords[0] == "friend" || mWords[0] == "template")
        return true;
    return false;
}

void Convertor::handlePost()
{
    if(mWords[1] == "static")
        mWords.erase(mWords.begin());
    if (mWords[0] == "explicit")
    {
        mWords.erase(mWords.begin());
        mType = Type::Ctor;
    }
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
    if(!mStruct)
    {
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
    }
    else
    {
        if(mType == Type::Ctor || mType == Type::Dtor)
        {
            mOutput << mClassName << "::" << mStructName << "::" << mWords[0] << mWords[1] << std::endl;
            mOutput << "{" << std::endl;
            mOutput << "    " << std::endl;
            mOutput << "}" << std::endl;
        }
        if(mType == Type::Void)
        {
            mOutput << mWords[0] << " " << mClassName << "::" << mStructName << "::" << mWords[1] << mWords[2] << std::endl;
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
            mOutput << mClassName << "::"  << mStructName << "::" << f << p << std::endl;
            mOutput << "{" << std::endl;
            mOutput << "    return ;" << std::endl;
            mOutput << "}" << std::endl;
        }
    }
	if(mType != Type::None)
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
