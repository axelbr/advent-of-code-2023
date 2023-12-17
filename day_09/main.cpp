#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <ranges>

std::vector<std::vector<long>> parse(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    std::vector<std::vector<long>> numbers;
    std::regex pattern("-?\\d+");
    while (std::getline(file, line)) {
        auto begin = std::sregex_iterator(line.begin(), line.end(), pattern);
        auto end = std::sregex_iterator();
        std::vector<long> row;
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            long n = std::stol(match[0]);
            row.push_back(n);
        }
        numbers.push_back(row);
    }
    return numbers;
}

long predict_next(const std::vector<long>& numbers) {
    std::vector<std::vector<long>> diffs;
    diffs.push_back(numbers);
    while (not std::all_of(diffs.back().begin(), diffs.back().end(), [](long n) { return n == 0; })) {
        auto last = diffs.back();
        std::vector<long> next;
        for (long i = 1; i < last.size(); ++i) {
            next.push_back(last[i] - last[i - 1]);
        }
        diffs.push_back(next);
    }
    long delta = 0;
    for (long order = diffs.size() - 1; order >= 0; order--) {
        long prev = diffs[order].back();
        long next = prev + delta;
        delta += prev;
        diffs[order].push_back(next);
    }
    return delta;
}

long task_1(const std::vector<std::vector<long>>& numbers) {
    long sum = 0;
    for (auto row: numbers) {
        long next = predict_next(row);
        sum += next;
    }
    return sum;
}

long task_2(const std::vector<std::vector<long>>& numbers) {
    long sum = 0;
    for (auto row: numbers) {
        std::reverse(row.begin(), row.end());
        long next = predict_next(row);
        sum += next;
    }
    return sum;
}

int main() {
    std::string input = "input.txt";
    auto numbers = parse(input);
    std::cout << "Task 1: " << task_1(numbers) << std::endl;
    std::cout << "Task 2: " << task_2(numbers) << std::endl;
}
