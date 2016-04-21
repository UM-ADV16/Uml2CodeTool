#ifndef UCODEGENERATIONVISITOR_H
#define UCODEGENERATIONVISITOR_H

#include "uVisitor.h"
#include "uLanguageStrategy.h"

/**
 * @brief The uCodeGenerationVisitor class generates all the code. It uses a Language Strategy to accomplish the generation of the code.
 * It is a singleton.
 */

class uCodeGenerationVisitor : public uVisitor
{
public:

    static uCodeGenerationVisitor& getInstance();
    void setLanguage(uLanguageStrategy * language);
    uLanguageStrategy * getLanguage() const;
    void setFileAttributes(std::string const& author, std::string const& date);
    void setUrl(std::string url);
    void cleanUrl();
    virtual void visit(uChildClass * childClass);
    virtual void visit(uBaseClass * baseClass);
    virtual void visit(uInterface * interfaceClass);
    virtual void visitSave(uChildClass * childClass, double x, double y);
    virtual void visitSave(uBaseClass * baseClass, double x, double y);
    virtual void visitSave(uInterface * interfaceClass, double x, double y);

    bool createFile(std::string const& name, std::string const& author, std::string const& date, std::string const& content, std::string const& lineComment, std::string const& path="");
    bool saveClassInDiagram(std::string const& name, std::string const& author, std::string const& date, std::string const& content, std::string const& path="");
    std::string createSaveAttributeString(uParameter * attribute);
    std::string createSaveMethodString(uMethod * method);

    std::string createContent(uInheritable * aClass, double x, double y, std::string const& base = "");
    std::string createChildInheritanceString(std::string const& base);
    std::string createBaseInheritanceString(uInheritable * aClass);

private:
    std::string url;
    uCodeGenerationVisitor();
    uCodeGenerationVisitor(const uCodeGenerationVisitor&);
    void operator=(uCodeGenerationVisitor const&);

    uLanguageStrategy * mLanguage;
    std::string mAuthor;
    std::string mDate;

    std::string getFileHeader(std::string const& fileName, std::string const& author, std::string const& date, std::string const& lineComment);

};

#endif // UCODEGENERATIONVISITOR_H
