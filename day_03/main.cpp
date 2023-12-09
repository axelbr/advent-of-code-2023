#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <numeric>

struct Schematics {
    std::vector<std::vector<char>> data;

    size_t width() const {
        if (this->data.empty()) {
            return 0;
        }
        return this->data[0].size();
    }

    size_t height() const {
        return this->data.size();
    }

    void parse(const std::string &path) {
        std::ifstream file(path);
        std::string line;
        std::vector<std::vector<char>> lines;
        while (std::getline(file, line)) {
            std::vector<char> row(line.begin(), line.end());
            lines.push_back(row);
        }
        this->data = lines;
    }

    char at(int x, int y) {
        return this->data[y][x];
    }

    std::vector<int> find_numbers_nearby(int x, int y) {
        std::vector<int> numbers;
        for (int i = y - 1; i <= y + 1; i++) {
            std::vector<char> row;
            for (int j = x - 1; j <= x + 1; j++) {
                if (i < 0 or j < 0 or i >= this->height() or j >= this->width()) {
                    continue;
                } else if (std::isdigit(this->at(j, i))) {
                    auto row = this->data[i];
                    std::string number;
                    int l = j - 1;
                    while (l >= 0 and std::isdigit(row[l])) {
                        number.insert(0, 1, row[l]);
                        l--;
                    }
                    while (j < row.size() and std::isdigit(row[j])) {
                        number += row[j];
                        j++;
                    }
                    int n = std::stoi(number);
                    numbers.push_back(n);
                }
            }

        }
        return numbers;
    }
};

int task_1(Schematics &schematics) {
    int sum = 0;
    for (int i = 0; i < schematics.height(); i++) {
        for (int j = 0; j < schematics.width(); j++) {
            auto symbol = schematics.at(j, i);
            if (symbol != '.' and !std::isdigit(symbol)) {
                auto numbers = schematics.find_numbers_nearby(j, i);
                sum += std::accumulate(numbers.begin(), numbers.end(), 0);
            }
        }
    }
    return sum;
}

int task_2(Schematics &schematics) {
    int sum = 0;
    for (int i = 0; i < schematics.height(); i++) {
        for (int j = 0; j < schematics.width(); j++) {
            auto symbol = schematics.at(j, i);
            if (symbol == '*') {
                auto numbers = schematics.find_numbers_nearby(j, i);
                if (numbers.size() == 2) {
                    sum += std::accumulate(numbers.begin(), numbers.end(), 1, std::multiplies<int>());
                }
            }
        }
    }
    return sum;
}

int main() {
    std::string input = "input.txt";
    Schematics schematics;
    schematics.parse(input);
    std::cout << "Task 1: " << task_1(schematics) << std::endl;
    std::cout << "Task 2: " << task_2(schematics) << std::endl;
}
