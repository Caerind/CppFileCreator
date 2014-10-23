/** //////////////////////////////////////////////////// **/
/// Project : -ProjectName-                              ///
/// File : --------.hpp                                  ///
/// Author : -RealName- -DevAlias-                       ///
/// Date : __/__/__                                      ///
/// Desc : This is a description of what my class do     ///
/** //////////////////////////////////////////////////// **/

#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <string>
#include <vector>

class TestFile;

class Test : public TestTest
{
    public:
        Test();
		~Test();
		
		// Test
		/* Test */
		
		typedef test test;
		
		#define test test;
		
		enum test
		{ 
			test = 0,
			test,
			test,
		}

        void a();
        int b();
        const& int c();

	virtual void render() =0;
	
    protected:
	void a(std::string a);
	int b(std:string const& b);
	std::string const& getTest(std::string const& s);
		
    private:
        std::string mFile;
	int a;
	bool b;
        std::vector<std::string> mWords;
        size_t mLineCount;
        Type mType;
};

#endif // TEST_HPP
