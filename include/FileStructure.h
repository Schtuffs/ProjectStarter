#pragma once

#include <string>
#include <vector>

constexpr const char* DEFAULT_LANG          = "C++";
constexpr const char* DEFAULT_TEMPLATE_SRC  = "/../templates";

class FileStructure {
public:
    FileStructure(std::string templatePath);
    
    // Allows for language to be changed
    bool ChangeLanguage(const std::string& lang);
    bool CopyFiles();
    void List();
    
    ~FileStructure();
    
private:
    std::string m_templatePath, m_exePath, m_chosenLanguage;
    std::vector<std::string> m_languages;

    bool CreateFolder(const std::string& src, const std::string& dest, const std::string& folder);
};

