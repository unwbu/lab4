#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <regex>

using namespace std;
using namespace std::filesystem;

void print_help() {
    cout << "Usage: count_files [options] [path]\n"
         << "Options:\n"
         << "  -h, --help           Show this help message\n"
         << "  -p, --pattern <pattern> Count files matching the pattern (e.g., \"\\.exe$\")\n";
}

int count_files_in_directory(const path& directory, const regex& pattern) {
    int count = 0;
    try {
        for (const auto& entry : directory_iterator(directory)) {
            if (is_regular_file(entry.status())) {
                if (regex_match(entry.path().filename().string(), pattern)) {
                    count++;
                }
            }
        }
    } catch (const filesystem_error& e) {
        cerr << "Error: " << e.what() << '\n';
        return -1;
    }
    return count;
}

int main(int argc, char* argv[]) {
    path directory = current_path();
    regex pattern(".*");

    if (argc > 1) {
        vector<string> args(argv + 1, argv + argc);
        for (size_t i = 0; i < args.size(); ++i) {
            if (args[i] == "-h" || args[i] == "--help") {
                print_help();
                return 0;
            } else if (args[i] == "-p" || args[i] == "--pattern") {
                if (i + 1 < args.size()) {
                    try {
                        pattern = regex(args[i + 1]);
                    } catch (const regex_error& e) {
                        cerr << "Error: Invalid regex pattern: " << args[i + 1] << '\n';
                        return 1;
                    }
                    i++;
                } else {
                    cerr << "Error: Missing pattern after " << args[i] << '\n';
                    return 1;
                }
            } else {
                directory = args[i];
            }
        }
    }

    int result = count_files_in_directory(directory, pattern);
    if (result >= 0) {
        cout << "Number of files: " << result << '\n';
        return 0; // Success
    } else {
        return 2; // Error
    }
}
