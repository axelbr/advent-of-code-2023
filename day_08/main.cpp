#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <set>
#include <map>
#include <ranges>

typedef std::map<std::string, std::vector<std::string>> Graph;

std::pair<Graph, std::string> parse(const std::string& path) {
    std::ifstream file(path);

    std::string instructions;
    std::getline(file, instructions);

    std::string line;
    std::regex pattern("([A-Z0-9]{3}) = \\(([A-Z0-9]{3}), ([A-Z0-9]{3})\\)");
    std::smatch matches;
    Graph graph;
    while (std::getline(file, line)) {
        if (std::regex_match(line, matches, pattern)) {
            auto node = matches[1].str();
            auto left = matches[2].str();
            auto right = matches[3].str();
            graph[node] = {left, right};
        }
    }
    return {graph, instructions};
}

int find_path_lengths(const Graph& graph, const std::string& instructions) {
    int num_steps = 0;
    std::map<std::string, int> path_lengths;

    for (auto &node: ) {
        auto current = node;
        for (size_t i = 0; i < instructions.size(); i = (i + 1) % instructions.size()) {
            auto neighbours = graph.at(current);
            if (instructions[i] == 'L') {
                current = neighbours[0];
            } else {
                current = neighbours[1];
            }
            num_steps++;
            if (current.ends_with("Z")) {
                path_lengths[node] = num_steps;
                break;
            }
    }
}

int solve(const Graph& graph, const std::string& instructions, const std::vector<std::string>& starting_nodes) {
    auto current = starting_nodes;
    std::vector<int> periodicities;
    std::transform(starting_nodes.begin(), starting_nodes.end(), std::back_inserter(periodicities), [&](const auto& node) {
        return find_periodicity(graph, node, instructions);
    });
    int num_steps = std::accumulate(periodicities.begin(), periodicities.end(), 1, std::lcm<int, int>);
    return num_steps;
}

int main() {
    std::string input = "input.txt";
    auto [graph, instructions] = parse(input);
    auto starting_nodes = std::vector<std::string>{"11A"};
    std::cout << "Task 1: " << solve(graph, instructions, starting_nodes) << std::endl;
    auto nodes = graph
                 | std::views::transform([](const auto& pair) { return pair.first; })
                 | std::views::filter([](const auto& node) { return node.ends_with("A"); });
    starting_nodes = std::vector<std::string>(nodes.begin(), nodes.end());
    std::cout << "Task 2: " << solve(graph, instructions, starting_nodes) << std::endl;
}