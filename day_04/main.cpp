#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <numeric>
#include <set>
#include <algorithm>
#include <regex>
#include <cmath>

struct Card {
    std::set<int> winning_numbers, drawn_numbers;
    int copies = 1;
};

std::vector<Card> parse_cards(const std::string &path) {
    std::ifstream file(path);
    std::string line;
    std::vector<Card> cards;
    std::regex pattern("\\d+");

    auto to_numbers = [&pattern](std::string &str) {
        auto begin = std::sregex_iterator(str.begin(), str.end(), pattern);
        auto end = std::sregex_iterator();
        std::vector<int> numbers;
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            int n = std::stoi(match[0]);
            numbers.push_back(n);
        }
        return numbers;
    };

    while (std::getline(file, line)) {
        auto winning = line.substr(line.find(":") + 1, line.find("|") - line.find(":") - 1);
        auto drawn = line.substr(line.find("|") + 1);
        auto winning_numbers = to_numbers(winning);
        auto drawn_numbers = to_numbers(drawn);
        Card card{
                .winning_numbers = {winning_numbers.begin(), winning_numbers.end()},
                .drawn_numbers = {drawn_numbers.begin(), drawn_numbers.end()}
        };
        cards.push_back(card);
    }
    return cards;
}

int task_1(const std::vector<Card> &cards) {
    std::vector<std::vector<int>> scores;
    std::transform(cards.begin(), cards.end(), std::back_inserter(scores), [](const Card &card) {
        std::vector<int> intersection;
        std::set_intersection(
                card.winning_numbers.begin(),
                card.winning_numbers.end(),
                card.drawn_numbers.begin(),
                card.drawn_numbers.end(),
                std::back_inserter(intersection)
        );
        return intersection;
    });
    return std::accumulate(scores.begin(), scores.end(), 0, [](int sum, const std::vector<int> &score) {
        if (score.empty()) {
            return sum;
        }
        return sum + (int) std::pow(2, score.size() - 1);
    });
}

int task_2(std::vector<Card> &cards) {
    for (int i = 0; i < cards.size(); i++) {
        auto &card = cards[i];
        std::vector<int> winning_numbers;
        std::set_intersection(
                card.winning_numbers.begin(),
                card.winning_numbers.end(),
                card.drawn_numbers.begin(),
                card.drawn_numbers.end(),
                std::back_inserter(winning_numbers)
        );
        for (int j = i + 1; j < cards.size() and j <= i + winning_numbers.size(); j++) {
            cards[j].copies += cards[i].copies;
        }
    }
    return std::accumulate(cards.begin(), cards.end(), 0, [](auto sum, auto &card) {
        return sum + card.copies;
    });
}

int main() {
    std::string input = "input.txt";
    auto cards = parse_cards(input);
    std::cout << "Task 1: " << task_1(cards) << std::endl;
    std::cout << "Task 2: " << task_2(cards) << std::endl;
}
