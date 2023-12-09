#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>
#include <numeric>
#include <ranges>

struct Draw {
    int red, green, blue;
};

struct Game {
    int id;
    std::vector<Draw> draws;
};


Game parse_game(const std::string &line) {
    int cur_pos = 0, next_pos = line.find(":");
    Game game;
    auto id = std::stoi(line.substr(5, next_pos - 5));
    game.id = id;
    while (cur_pos < line.size() and cur_pos > -1) {
        next_pos = line.find(";", cur_pos + 1);
        auto hand = line.substr(cur_pos + 1, next_pos - cur_pos);
        cur_pos = next_pos;
        std::regex pattern("((\\d+)\\s(red|green|blue)){1,3}");
        auto begin = std::sregex_iterator(hand.begin(), hand.end(), pattern);
        auto end = std::sregex_iterator();
        Draw draw{0, 0, 0};
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            int n = std::stoi(match[2]);
            auto color = match[3];
            if (color == "red") {
                draw.red = n;
            } else if (color == "green") {
                draw.green = n;
            } else {
                draw.blue = n;
            }
        }
        game.draws.push_back(draw);
    }
    return game;
}

std::vector<Game> parse_games(const std::string &path) {
    std::ifstream file(path);
    std::string line;
    std::vector<Game> games;
    while (std::getline(file, line)) {
        auto game = parse_game(line);
        games.push_back(game);
    }
    return games;
}

int task_1(const std::vector<Game> &games) {
    Draw bag{.red = 12, .green = 13, .blue = 14};
    auto is_possible = [&bag](const Game &game) {
        bool possible = true;
        for (auto &hand: game.draws) {
            possible = possible && hand.red <= bag.red && hand.blue <= bag.blue && hand.green <= bag.green;
        }
        return possible;
    };
    auto filtered = std::views::filter(games, is_possible);
    int sum = 0;
    for (auto &game: filtered) {
        sum += game.id;
    }
    return sum;
}

int task_2(const std::vector<Game> &games) {
    std::vector<Draw> min_bags;
    std::transform(games.begin(), games.end(), std::back_inserter(min_bags), [&](const Game &game) {
        Draw init{0, 0, 0};
        return std::accumulate(game.draws.begin(), game.draws.end(), init, [](auto a, auto &b) {
            return Draw{
                    .red = std::max(a.red, b.red),
                    .green = std::max(a.green, b.green),
                    .blue = std::max(a.blue, b.blue)
            };
        });
    });
    auto answer = std::accumulate(min_bags.begin(), min_bags.end(), 0, [](auto carry, auto &hand) {
        return carry + hand.red * hand.green * hand.blue;
    });
    return answer;
}

int main() {
    std::string input = "input.txt";
    auto games = parse_games(input);
    std::cout << "Task 1: " << task_1(games) << std::endl;
    std::cout << "Task 2: " << task_2(games) << std::endl;
}
