#include <iostream>
#include <string>
#include <fstream>


void string_search(const std::string& str, const std::string& pattern) {
    int s_len = str.length();
    int p_len = pattern.length();
    bool found = false;

    for (int i = 0; i <= s_len - p_len; i++) {
        int j = 0;
        while (j < p_len && str[i + j] == pattern[j]) {
            j++;
        }
        if (j == p_len) { // If we have reached the end of the pattern
            found = true;
            std::cout << "Found pattern at index: " << i << std::endl;
            break;
        }
    }

    if (!found)
        std::cout << "Could not find the pattern" << std::endl;
}

// Driver function
int main() {
    std::ifstream file("KJB.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1;
    }

    std::string str;
    std::getline(file, str, '\0'); // Read the entire file into the string
    file.close();

    std::string pattern = "revolters";

    string_search(str, pattern);

    return 0;
}

