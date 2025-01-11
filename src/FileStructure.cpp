#include "FileStructure.h"

#include <filesystem>
#include <fstream>

FileStructure::FileStructure() {
    // Auto select C/CPP
    this->m_languageMap = { {" ", INVALID}, {"C", CPP}, {"CPP", CPP} };
    this->m_chosenLanguage = CPP;
}

FileStructure::~FileStructure() {
    // Nothing todo
}

    // Allows for language to be changed
bool FileStructure::ChangeLanguage(const std::string& lang) {
    Language l = this->m_languageMap[lang];
    if (l == INVALID) {
        std::cout << "Unsupported language\n";
        return false;
    }
    // Valid language, can be set
    this->m_chosenLanguage = l;
    return true;
}

bool FileStructure::CreateFiles() {
    // Choose language blueprint
    switch(this->m_chosenLanguage) {
    case CPP:
        return this->CreateFilesCpp();
    default:
        return false;
    }
}

bool FileStructure::CreateFilesCpp() {
    // Returns are inverted so they can be or'd onto the final response
    // Allows for 1 variable to track all datapoints
    bool createDirectory = !std::filesystem::create_directory("bin");
    createDirectory |= !std::filesystem::create_directory("include");
    createDirectory |= !std::filesystem::create_directory("lib");
    createDirectory |= !std::filesystem::create_directory("src");

    // Flip true/false
    createDirectory = !createDirectory;

    // Create sub files
    if (createDirectory) {
        // Create main file
        std::filesystem::current_path("./src");
        std::ofstream mainfile("main.cpp");
        if (mainfile.is_open()) {
            mainfile
            << "#include <iostream>\n\n"
            << "int main(int argc, char** argv) {\n"
            << "\treturn 0;\n"
            << "}\n";
            
            mainfile.close();
        }

        // Create makefile
        std::filesystem::current_path("../bin");
        std::ofstream makefile("Makefile");
        if (makefile.is_open()) {
            makefile
            << "CXX		 = g++\n\n"
            << "EXE		 = CHANGEME\n\n"
            << "SRC		 = ../src\n"
            << "INCLUDE	 = ../include\n"
            << "LIB		 = ../lib\n\n"
            << "FLAGS	 = -std=c++20 -I$(INCLUDE) -L$(LIB)\n"
            << "CXXFLAGS = -c -Wall $(FLAGS)\n"
            << "LDFLAGS	 = $(FLAGS) -o\n\n"
            << "OBJECTS	 = main.o\n\n"
            << "all: $(OBJECTS)\n"
            << "\t$(CXX) $(OBJECTS) $(LDFLAGS) $(EXE)\n\n"
            << "main.o: $(SRC)/main.cpp\n"
            << "\t$(CXX) $(CXXFLAGS) $<\n\n"
            << "clean:\n";
            #ifdef _WIN32
            makefile << "\tdel -q *.exe *.o\n";
            #elif __linux__
            makefile << "\t rm *.o $(EXE)";
            #endif
            
            makefile.close();
        }
    }

    // Final or to account for previous flips
    return createDirectory;
}

