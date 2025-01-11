#include <iostream>
#include <vector>

#include "FileStructure.h"

#define VERSION         "1.0"
#define CODE_EXIT       0
#define CODE_INVALID    -1

// Changes the args to strings
std::vector<std::string> Parse(int argc, char** argv) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }
    return args;
}

// Check for command in args, returns the index
// Returns CODE_INVALID if index not found
int HasCommand(const std::vector<std::string>& args, const std::string& command) {
    for (int i = 0; i < (int)(args.size()); i++) {
        if (args[i].compare(command) == 0) {
            return i;
        }
    }
    return CODE_INVALID;
}

// Print help menu
void Help() {
    std::cout << "\n  Commands\n";
    std::cout << "-h        Prints this menu\n";
    std::cout << "-v        Prints the program version\n";
    std::cout << "-l        Lists valid languages. Languages are all capitalized\n";
    std::cout << "          Have language in argument after to select it instead.\n";
    std::cout << "-r        Allows running with default params\n";
    std::cout << std::endl;
}

// Version info
void Version() {
    std::cout << "\nVersion " << VERSION << "\n";
    std::cout << "Created to create my filesystems quicker\n";
    std::cout << "Changes can be made for the filesystem based on preferences\n";
    std::cout << std::endl;
}

// Prints valid languages
void Languages() {
    std::cout << "\n  Languages\n";
    std::cout << "C/CPP\n";
    std::cout << std::endl;
}

// Perform the command line arguments
bool Commands(FileStructure& fs, std::vector<std::string>& args) {
    // Each check
    int index = CODE_INVALID;
    bool run = false;

    // Version info
    if ((index = HasCommand(args, "-v")) != CODE_INVALID) {
        Version();
    }

    // Help menu
    if ((index = HasCommand(args, "-h")) != CODE_INVALID || args.size() == 0) {
        Help();
    }

    // Languages
    if ((index = HasCommand(args, "-l")) != CODE_INVALID) {
        // Check if next arg is also an arg or language
        if (index != (int)(args.size()) - 1 && args[index + 1][0] != '-') {
            if (fs.ChangeLanguage(args[index + 1])) {
                std::cout << "Language set to " << args[index + 1] << "\n";
                run = true;
            } 
            else {
                std::cout << "Language could not be set to " << args[index + 1] << ". Perhaps try all CAPS\n";
            }
        }
        else {
            Languages();
        }
    }

    // Run with default settings
    if (HasCommand(args, "-r") != CODE_INVALID) {
        run = true;
    }

    return run;
}


int main(int argc, char** argv) {
    // Setup
    auto args = Parse(argc, argv);

    // Parsing commands
    FileStructure fs;
    if (!Commands(fs, args)) {
        return 0;
    }

    // Create folders
    std::cout << "Creating folders\n";
    if (fs.CreateFiles()) {
        std::cout << "Successfully created folders\n";
    }
    else {
        std::cout << "Failed to create folders\n";
    }
    
    return 0;
}

