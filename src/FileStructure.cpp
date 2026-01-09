#include "FileStructure.h"

#include <filesystem>
#include <iostream>

FileStructure::FileStructure(std::string srcPath) {
    this->m_exePath = std::filesystem::current_path().string();
    
    for (int i = srcPath.length() - 1; i > 0; i--) {
        // Removes the executable
        if (srcPath[i] == '\\' || srcPath[i] == '/') {
            srcPath = srcPath.substr(0, i);
            break;
        }
    }
    // Set path for checking templates
    this->m_templatePath = srcPath + DEFAULT_TEMPLATE_SRC;

    // Reads folders in path to determine valid languages
    for (const auto& entry : std::filesystem::directory_iterator(this->m_templatePath)) {
        if (entry.is_directory()) {
            // Create entry and remove path
            std::string folder = entry.path().string();
            folder = folder.substr(this->m_templatePath.length());

            // Remove extra slash
            if (folder[0] == '\\' || folder [0] == '/') {
                folder = folder.substr(1);
            }

            // Add to languages
            this->m_languages.push_back(folder);
        }
    }

    // Always comes with a cpp system
    this->m_chosenLanguage = DEFAULT_LANG;
}

FileStructure::~FileStructure() {
    // Nothing todo
}

bool FileStructure::ChangeLanguage(const std::string& lang) {
    // Loop through all languages
    for (const std::string& m_language : this->m_languages) {
        // Check if language is in list
        if (lang.compare(m_language) == 0) {
            this->m_chosenLanguage = lang;
            return true;
        }
    }

    // No language folder was found
    std::cout << "\nTemplate \"" << lang << "\" not found.\n";
    return false;
}

bool FileStructure::CopyFiles() {
    // Setup data for transfering
    std::string src = this->m_templatePath + "/" + this->m_chosenLanguage;
    std::string dest = this->m_exePath;

    // Read all folder names in folder
    std::vector<std::string> folders;
    for (const auto& entry : std::filesystem::directory_iterator(src)) {
        // Create entry and remove path
        std::string folder = entry.path().string();
        folder = folder.substr(src.length());
        

        // Remove extra slash
        if (folder[0] == '\\' || folder [0] == '/') {
            folder = folder.substr(1);
        }

        // Ignore build or winbuild for cmake
        if (std::string("build").compare(folder) == 0 || 
                std::string("winbuild").compare(folder) == 0) {
            continue;
        }
        
        folders.push_back(folder);
    }
    
    // Copy system, windows gcc implementation is faulty.
    // Cannot just use copy to overwrite previous files.
    for (const std::string& folder : folders) {
        if (!this->CreateFolder(src, dest, folder)) {
            std::cout << "\nFailed to create the filesystem\n";
            return false;
        }
    }

    return true;
}

bool FileStructure::CreateFolder(const std::string& src, const std::string& dest, const std::string& folder) {
    try {
        // Add the folder into working directory
        std::filesystem::copy_options options = std::filesystem::copy_options::recursive;
        std::filesystem::copy(src + "/" + folder, dest + "/" + folder, options);
    } catch (std::filesystem::filesystem_error& e) {
        return false;
    }
    return true;
}

void FileStructure::List() {
    for (const std::string& lang : this->m_languages) {
        std::cout << lang << "\n";
    }
}

