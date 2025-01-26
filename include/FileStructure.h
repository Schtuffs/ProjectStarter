#pragma once

#include <iostream>
#include <string>
#include <vector>

#define DEFAULT_LANG            "C++"
#define DEFAULT_TEMPLATE_SRC    std::string("/../templates")

class FileStructure {
private:
    std::string m_templatePath, m_exePath, m_chosenLanguage;
    std::vector<std::string> m_languages;

    bool CreateFolder(const std::string& src, const std::string& dest, const std::string& folder);

public:
    FileStructure(std::string templatePath);

    // Allows for language to be changed
    bool ChangeLanguage(const std::string& lang);
    bool CopyFiles();
    void List();

    ~FileStructure();
};

