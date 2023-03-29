#include <iostream>
#include <fstream>
#include <string>
#include <map>

int main() {
    std::string input_filename = "input.txt";
    std::string output_filename = "output.txt";
    std::ifstream input_file(input_filename);
    std::ofstream output_file(output_filename);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open input file " << input_filename << std::endl;
        return 1;
    }
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output file " << output_filename << std::endl;
        return 1;
    }
    std::map<std::string, int> word_counts;
    std::string line;
    while (std::getline(input_file, line)) {
        size_t start_pos = 0;
        size_t end_pos = 0;
        while (end_pos != std::string::npos) {
            end_pos = line.find_first_of(" \t\n\r\f\v", start_pos);
            std::string word = line.substr(start_pos, end_pos - start_pos);
            if (!word.empty()) {
                ++word_counts[word];
            }
            start_pos = line.find_first_not_of(" \t\n\r\f\v", end_pos);
        }
    }
    input_file.close();
    output_file << "Word\tCount\n";
    for (auto const& [word, count] : word_counts) {
        output_file << word << "\t" << count << "\n";
    }
    output_file.close();
    return 0;
}
