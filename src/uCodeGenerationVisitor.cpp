#include "uCodeGenerationVisitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "uDebugPrinter.h"

using namespace std;

/**
 * @brief uCodeGenerationVisitor::uCodeGenerationVisitor, the constructor stablishes the Author and the Date. This could be changed with
 * "setFileAttributes" but it is common for all the classes.
 */

uCodeGenerationVisitor::uCodeGenerationVisitor()
{
    mAuthor = "";
    mDate = "";
}

uCodeGenerationVisitor &uCodeGenerationVisitor::getInstance()
{
    static uCodeGenerationVisitor instance;
    return instance;
}

void uCodeGenerationVisitor::setLanguage(uLanguageStrategy * language)
{
    mLanguage = language;
}

uLanguageStrategy * uCodeGenerationVisitor::getLanguage() const
{
    return mLanguage;
}

void uCodeGenerationVisitor::setFileAttributes(const string &author, const string &date)
{
    mAuthor = author;
    mDate = date;
}

void uCodeGenerationVisitor::visit(uChildClass *childClass)
{
    if (mLanguage->hasSeparateFiles()) {
        createFile(childClass->getName() + mLanguage->getImplementationFileExtension(), mAuthor, mDate, mLanguage->createImplementationFileContent(childClass, childClass->getParent()->getName()), mLanguage->getLineComment());
        cout << "    " << childClass->getName() + mLanguage->getImplementationFileExtension() << endl;
    }
    createFile(childClass->getName() + mLanguage->getDeclarationFileExtension(), mAuthor, mDate, mLanguage->createDeclarationFileContent(childClass, childClass->getParent()->getName()), mLanguage->getLineComment());
    cout << "    " << childClass->getName() + mLanguage->getDeclarationFileExtension() << endl;
}

void uCodeGenerationVisitor::visit(uBaseClass *baseClass)
{
    if (mLanguage->hasSeparateFiles()) {
        createFile(baseClass->getName() + mLanguage->getImplementationFileExtension(), mAuthor, mDate, mLanguage->createImplementationFileContent(baseClass), mLanguage->getLineComment());
        cout << "    " << baseClass->getName() + mLanguage->getImplementationFileExtension() << endl;
    }
    createFile(baseClass->getName() + mLanguage->getDeclarationFileExtension(), mAuthor, mDate, mLanguage->createDeclarationFileContent(baseClass), mLanguage->getLineComment());
    cout << "    " << baseClass->getName() + mLanguage->getDeclarationFileExtension() << endl;
}

void uCodeGenerationVisitor::visit(uInterface *interfaceClass)
{
    if (mLanguage->hasSeparateFiles()) {
        createFile(interfaceClass->getName() + mLanguage->getImplementationFileExtension(), mAuthor, mDate, mLanguage->createImplementationFileContent(interfaceClass), mLanguage->getLineComment());
        cout << "    " << interfaceClass->getName() + mLanguage->getImplementationFileExtension() << endl;
    }
    createFile(interfaceClass->getName() + mLanguage->getDeclarationFileExtension(), mAuthor, mDate, mLanguage->createDeclarationFileContent(interfaceClass), mLanguage->getLineComment());
    cout << "    " << interfaceClass->getName() + mLanguage->getDeclarationFileExtension() << endl;
}
string uCodeGenerationVisitor::createContent(uInheritable * aClass, string const& base)
{

        stringstream fileContent;

        // add imports
        // TODO

        // class name + inheritance
        if (base != "")
            fileContent << getAccessString(aClass->getAccess()) << " class " << aClass->getName() << " implements " << base << "{" << endl << endl;
        else
            fileContent << getAccessString(aClass->getAccess()) << " class " << aClass->getName() << " {" << endl << endl;

        // attributes
        TParameters attributes = aClass->getAttributes();
        for (TParametersIter iter = attributes.begin(); iter < attributes.end(); ++iter) {
            //fileContent << "\t" << createAttributeDeclaration(*iter) << endl << endl;
            cout << *iter << " ";
        }

        // methods
        TMethods methods = aClass->getMethods();
        for (TMethodsIter iter = methods.begin(); iter < methods.end(); ++iter) {
            //fileContent << "\t" << createMethodDeclaration(*iter) << endl;
            cout << *iter << " ";
        }

        fileContent << "};" << endl;

        return fileContent.str();

}

//save visit functions
void uCodeGenerationVisitor::visitSave(uChildClass *childClass)
{

    saveDiagram(childClass->getName(), mAuthor, mDate, createContent(childClass, childClass->getParent()->getName()));
    cout << "    " << childClass->getName()  << endl;
}

void uCodeGenerationVisitor::visitSave(uBaseClass *baseClass)
{

    saveDiagram(baseClass->getName(), mAuthor, mDate, createContent(baseClass));
    cout << "    " << baseClass->getName()  << endl;
}

void uCodeGenerationVisitor::visitSave(uInterface *interfaceClass)
{

    saveDiagram(interfaceClass->getName(), mAuthor, mDate, createContent(interfaceClass));
    cout << "    " << interfaceClass->getName()  << endl;
}

bool uCodeGenerationVisitor::createFile(string const& name, string const& author, string const& date, string const& content, string const& lineComment, string const& path)
{
    ofstream myfile;
    myfile.open(path+name.c_str());
    if (!myfile.is_open())
        return false;

    myfile << getFileHeader(name, author, date, lineComment);
    myfile << content;
    myfile.close();
    //saveDiagram(name, author, date, content, lineComment, path);

    return true;
}
//method to save the diagram to a file using JSON objects to represent them
bool uCodeGenerationVisitor::saveDiagram(string const& name, string const& author, string const& date, string const& content, string const& path)
{
    ofstream myfile;
    const string & temp = "/tmp/";
    myfile.open(temp+name.c_str() + ".uct");
    if (!myfile.is_open())
        return false;
//this will work but i need it do work a bit differently than this because i only want one file.
    myfile << "\{\"classes\" : [\n";
    myfile << "{\"name\":\"" +name + "\",\n";
    myfile << "\"author\":\"" + author + "\",\n";
    myfile << "\"date\":\"" + date + "\",\n";
    myfile << "\"content\":\"" + content + "\"}}";
    myfile.close();
    return true;
}
string uCodeGenerationVisitor::getFileHeader(std::string const& fileName, std::string const& author, std::string const& date, std::string const& lineComment)
{
    string header = "";
    header += lineComment + " ------------------------------------------------------------------\n";
    header += lineComment + " File created with uCode - https://github.com/dstoeg/Uml2CodeTool\n";
    header += lineComment + " ------------------------------------------------------------------\n";
    header += lineComment + " file      : " + fileName + "\n";
    header += lineComment + " author    : " + author + "\n";
    header += lineComment + " created   : " + date   + "\n";
    header += lineComment + " ------------------------------------------------------------------\n\n";

    return header;
}
