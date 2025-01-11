#pragma once

#include <iostream>
#include <map>

enum Language { INVALID, CPP };

class FileStructure {
private: 
    Language    m_chosenLanguage;
    std::map<std::string, Language> m_languageMap;

    // Language based file creation

    bool CreateFilesCpp();

public:
    FileStructure();

    // Allows for language to be changed
    bool ChangeLanguage(const std::string& lang);
    bool CreateFiles();

    ~FileStructure();
};

