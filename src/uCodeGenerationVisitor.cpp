#include "uCodeGenerationVisitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include "uDebugPrinter.h"

using namespace std;

/**
 * @brief uCodeGenerationVisitor::uCodeGenerationVisitor, the constructor establishes the Author and the Date. This could be changed with
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
void uCodeGenerationVisitor::setUrl(std::string thisurl)
{
    url = thisurl;
}

void uCodeGenerationVisitor::cleanUrl()
{
    //TODO
    ofstream myfile;
    const string substring = url.substr(7, url.length());
    uDebugPrinter::printText("Path: " + substring);
    myfile.open(substring);
    uDebugPrinter::printText(substring);
    myfile << "";
    myfile.close();
}

void uCodeGenerationVisitor::visit(uChildClass *childClass)
{
    if (mLanguage->hasSeparateFiles()) {
        createFile(childClass->getName() + mLanguage->getImplementationFileExtension(), mAuthor, mDate, mLanguage->createImplementationFileContent(childClass, childClass->getParent()->getName()), mLanguage->getLineComment(), url);
        cout << "    " << childClass->getName() + mLanguage->getImplementationFileExtension() << endl;
    }
    createFile(childClass->getName() + mLanguage->getDeclarationFileExtension(), mAuthor, mDate, mLanguage->createDeclarationFileContent(childClass, childClass->getParent()->getName()), mLanguage->getLineComment(), url);
    cout << "    " << childClass->getName() + mLanguage->getDeclarationFileExtension() << endl;
}

void uCodeGenerationVisitor::visit(uBaseClass *baseClass)
{
    if (mLanguage->hasSeparateFiles()) {
        createFile(baseClass->getName() + mLanguage->getImplementationFileExtension(), mAuthor, mDate, mLanguage->createImplementationFileContent(baseClass), mLanguage->getLineComment(), url);
        cout << "    " << baseClass->getName() + mLanguage->getImplementationFileExtension() << " url: " << url << endl;
    }
    createFile(baseClass->getName() + mLanguage->getDeclarationFileExtension(), mAuthor, mDate, mLanguage->createDeclarationFileContent(baseClass), mLanguage->getLineComment(), url);
    cout << "    " << baseClass->getName() + mLanguage->getDeclarationFileExtension() << endl;
}

void uCodeGenerationVisitor::visit(uInterface *interfaceClass)
{
    if (mLanguage->hasSeparateFiles()) {
        createFile(interfaceClass->getName() + mLanguage->getImplementationFileExtension(), mAuthor, mDate, mLanguage->createImplementationFileContent(interfaceClass), mLanguage->getLineComment(), url);
        cout << "    " << interfaceClass->getName() + mLanguage->getImplementationFileExtension() << endl;
    }
    createFile(interfaceClass->getName() + mLanguage->getDeclarationFileExtension(), mAuthor, mDate, mLanguage->createDeclarationFileContent(interfaceClass), mLanguage->getLineComment(), url);
    cout << "    " << interfaceClass->getName() + mLanguage->getDeclarationFileExtension() << endl;
}
string uCodeGenerationVisitor::createSaveAttributeString(uParameter * attribute)
{
    string attributeString = getAccessString(attribute->getAccess()) + " " + attribute->getType() + " " + attribute->getName();
    return attributeString;
}
std::string uCodeGenerationVisitor::createSaveMethodString(uMethod * method)
{
    string methodStr = "{\"method\" : \"";
    TParameters params = method->getParameters();
    methodStr += getAccessString(method->getAccess()) + " " + method->getReturnType() + " " + method->getName() + "\"},\n";
    methodStr += "\t{\"parameters\" : \"";
    if (params.size() > 0) {
        for (TParametersIter iter = params.begin(); iter < params.end()-1; ++iter) {
            methodStr += (*iter)->getType() + " " + (*iter)->getName() + ", ";
        }
        methodStr += params[params.size()-1]->getType() + " " + params[params.size()-1]->getName();
    }
    return methodStr + "}\n";
}
//create an inheritance string for a child class
std::string uCodeGenerationVisitor::createChildInheritanceString(string const& base)
{
    std::string baseWords = "\n\t\"parent\" : \"" + base + "\",\n";
    baseWords += "\t\"interface\": \"false\",\n";
    baseWords += "\t\"abstract\": \"false\",\n";
    return baseWords;
}
//create a inheritance string for  a base class
std::string uCodeGenerationVisitor::createBaseInheritanceString(uInheritable * aClass)
{
    std::string baseWords = "\n\t\"parent\" : \"\",\n";
    if (aClass->isInterface())
    {
        baseWords += "\t\"interface\": \"true\",\n";
    }
    else
    {
        baseWords += "\t\"interface\": \"false\",\n";
    }
    if (aClass->isAbstract())
    {
        baseWords += "\t\"abstract\": \"true\",\n";
    }
    else
    {
         baseWords += "\t\"abstract\": \"false\",\n";
    }

    return baseWords;
}



string uCodeGenerationVisitor::createContent(uInheritable * aClass, double x, double y, string const& base)
{
    uDebugPrinter::printText("createing content mehtod" + std::to_string(x));
    stringstream fileContent;
//iterate through all the attributes associated with the current class and retrun the parameter and methods in a string
        TParameters attributes = aClass->getAttributes();
        fileContent << "\t{\"attributes\":[" << endl;
        for (TParametersIter iter = attributes.begin(); iter < attributes.end(); ++iter) {
            if (iter < attributes.end()-1)
            {
                fileContent << "\t\t{\"attribute\" : \"" << createSaveAttributeString(*iter) << "\"}," << endl;
            }
            else
            {
                fileContent << "\t\t{\"attribute\" : \"" << createSaveAttributeString(*iter) << "\"}" << endl;
            }
        }
        fileContent << "\t\t]},\n \t{\"methods\" : [" << endl;
        // methods
        TMethods methods = aClass->getMethods();
        for (TMethodsIter iter = methods.begin(); iter < methods.end(); ++iter) {
            if (iter < methods.end()-1)
            {
                fileContent << "\t\t" << createSaveMethodString(*iter)  << endl;
            }
            else
            {
                fileContent << "\t\t" << createSaveMethodString(*iter) << endl;
            }
            cout << *iter << " ";
        }

        //need parent info here
        fileContent << "\t]}";
        fileContent << "\t{\"x\" : \"" + std::to_string(x) + "\" }\n";
        fileContent << "\t{\"y\" : \"" + std::to_string(y) + "\" }\n";
        //now we need some cheking to see what kind of class we have
        bool hasParent = aClass->hasParent();
        if (hasParent)
        {
            fileContent << createChildInheritanceString(base);
        }
        else
        {
            fileContent << createBaseInheritanceString(aClass);
        }
        return fileContent.str();

}

//save visit functions one for each type of class that could be visited.
void uCodeGenerationVisitor::visitSave(uChildClass *childClass, double x, double y)
{

  if (!childClass->hasParent())
        saveClassInDiagram(childClass->getName(), mAuthor, mDate, createContent(childClass, x, y), url);
    else
        saveClassInDiagram(childClass->getName(), mAuthor, mDate, createContent(childClass, x, y, childClass->getParent()->getName()), url);

}

void uCodeGenerationVisitor::visitSave(uBaseClass *baseClass, double x, double y)
{

    saveClassInDiagram(baseClass->getName(), mAuthor, mDate, createContent(baseClass, x, y), url);

}

void uCodeGenerationVisitor::visitSave(uInterface *interfaceClass, double x, double y)
{

    saveClassInDiagram(interfaceClass->getName(), mAuthor, mDate, createContent(interfaceClass, x, y), url);

}

bool uCodeGenerationVisitor::createFile(string const& name, string const& author, string const& date, string const& content, string const& lineComment, string const& path)
{
    //this is clearly not ok for the main branch
      uDebugPrinter::printText("new path " + path+name.c_str());
    const string thisPath = path.substr(7, path.length());
    uDebugPrinter::printText("new path " + thisPath+"/"+name.c_str());
    ofstream myfile;
    myfile.open(thisPath+"/"+name.c_str());
    if (!myfile.is_open())
        return false;

    myfile << getFileHeader(name, author, date, lineComment);
    myfile << content;
    myfile.close();

    return true;
}
//method to save the diagram to a file using JSON objects to represent them
bool uCodeGenerationVisitor::saveClassInDiagram(string const& name, string const& author, string const& date, string const& content, string const& path)
{
    ofstream myfile;
    bool isOpen = false;
    const string substring = path.substr(7, path.length());
    uDebugPrinter::printText("Path: " + substring);
//    myfile.open(temp+name.c_str() + ".uct", ios::app);
    ifstream ifile(substring);


    if (ifile) {
            isOpen = true;
            std::cout << "did  exist"<< std::endl;
        } else {
            isOpen = false;
            std::cout << "did not exist"<< std::endl;
        }

    myfile.open(substring, ios::app);
    uDebugPrinter::printText(substring);
    if (!myfile.is_open())
        return false;

    myfile << "\{\"classes\" : [\n";
    myfile << "\t{\"name\":\"" +name + "\"},\n";
    myfile << "\t{\"author\":\"" + author + "\"},\n";
    myfile << "\t{\"date\":\"" + date + "\"},\n";
    myfile << "\t{\"content\": " + content + "}\n";
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
