#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int to_number(const std::string& lhs, const std::string& rhs) {
    std::vector<std::string> numbers = {
        "one", "two", "three", "four",
        "five", "six", "seven", "eight", "nine"
    };

    auto to_digit = [&numbers](const std::string& str) -> std::string {
        if (str.length() > 1) {
            auto idx = std::find(numbers.begin(), numbers.end(), str);
            int dist = (int) std::distance(numbers.begin(), idx);
            return std::to_string(dist + 1);
        }
        return str;
    };
    std::string number{to_digit(lhs) + to_digit(rhs)};
    return std::stoi(number);
}


int solve(const std::string& file, bool part_two = false) {
    std::ifstream input(file);
    std::string line;
    std::vector<std::string> numbers = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
    };
    if (part_two) {
        std::vector<std::string> text_numbers = {
            "one", "two", "three", "four",
            "five", "six", "seven", "eight", "nine"
        };
        numbers.insert(numbers.end(), text_numbers.begin(), text_numbers.end());
    }
    int sum = 0;
    while (std::getline(input, line)) {
        int first_pos = (int) line.size(), last_pos = 0;
        std::string first, last;
        for (auto& n: numbers) {
            auto pos = line.find(n);
            if (pos != std::string::npos and pos <= first_pos) {
                first_pos = (int) pos;
                first = n;
            }
            auto rpos = line.rfind(n);
            if (rpos != std::string::npos and rpos >= last_pos) {
                last_pos = (int) rpos;
                last = n;
            }
        }
        int n = to_number(first, last);
        sum += n;
    }
    return sum;
}


int main() {
    std::string file = "input.txt";
    std::cout << "Task 1: " << solve(file, false) << std::endl;
    std::cout << "Task 2: " << solve(file, true) << std::endl;
    return 0;
}
