#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <regex>


long solve(std::vector<std::pair<long, long>> results) {
    long answer = 1;
    for (auto [time, distance]: results) {
        long num_of_winning_strategies = 0;
        for (long t = 0; t < time; t++) {
            if (t * (time - t) > distance) {
                num_of_winning_strategies++;
            }
        }
        answer *= num_of_winning_strategies;
    }
    return answer;
}

std::vector<std::pair<long, long>> parse(const std::string &path) {
    auto extract_numbers = [](const std::string &line) {
        std::regex re(R"(\d+)");
        std::smatch match;
        std::vector<long> numbers;
        std::string current = line;
        while (std::regex_search(current, match, re)) {
            numbers.push_back(std::stol(match.str()));
            current = match.suffix();
        }
        return numbers;
    };
    std::ifstream file(path);
    std::string line;
    std::vector<std::pair<long, long>> result;
    std::getline(file, line);
    auto times = extract_numbers(line);
    std::getline(file, line);
    auto distances = extract_numbers(line);
    for (long i = 0; i < times.size(); i++) {
        result.push_back({times[i], distances[i]});
    }
    return result;
}

int main() {
    std::vector<std::string> inputs = {
            "task_01.txt", "task_02.txt"
    };
    for (int i = 0; i < inputs.size(); i++) {
        auto results = parse(inputs[i]);
        std::cout << "Task " << i + 1 << ": " << solve(results) << std::endl;
    }
}
