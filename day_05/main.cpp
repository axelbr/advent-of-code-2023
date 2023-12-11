#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <numeric>

struct Range {
    long start, end;


    bool contains(Range other) {
        return this->start <= other.start and this->end >= other.end;
    }

    Range translate(const Range &source, const Range &destination) {
        long start = destination.start + this->start - source.start;
        return Range{start, start + this->length()};
    }

    long length() {
        return this->end - this->start;
    }
};


struct Mapping {
    std::vector<std::pair<Range, Range>> mappings;
    std::vector<Range> apply(Range range) {
        std::vector<Range> result;
        for (int i = 0; i < this->mappings.size(); i++) {
            auto [source, target] = this->mappings[i];
            if (source.contains(range)) {
                result.push_back(range.translate(source, target));
                return result;
            } else if (range.start >= source.start and range.start <= source.end and range.end > source.end) {
                Range left{range.start, source.end};
                Range right{source.end + 1, range.end};
                result.push_back(left.translate(source, target));
                range = right;
            }
        }
        result.push_back(range);
        return result;
    }
};

std::pair<std::vector<Mapping>, std::vector<long>> parse(const std::string &path) {
    std::vector<Mapping> almanac;
    std::vector<long> seeds;
    std::ifstream file(path);
    std::string line;
    std::getline(file, line);
    std::regex pattern("\\d+");
    std::smatch match;
    auto begin = std::sregex_iterator(line.begin(), line.end(), pattern);
    for (std::sregex_iterator i = begin; i != std::sregex_iterator(); ++i) {
        match = *i;
        long n = std::stol(match[0]);
        seeds.push_back(n);
    }

    std::regex mapping_pattern("[a-z\\-]+ map:");
    std::regex range_pattern("(\\d+) (\\d+) (\\d+)");
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        std::smatch match;
        if (std::regex_match(line, match, mapping_pattern)) {
            std::vector<std::pair<Range, Range>> mappings;
            while (getline(file, line) and !line.empty()) {
                std::smatch mapping_match;
                if (std::regex_match(line, mapping_match, range_pattern)) {
                    auto source_start = std::stol(mapping_match[2]);
                    auto destination_start = std::stol(mapping_match[1]);
                    auto length = std::stol(mapping_match[3]);
                    Range source_range{source_start, source_start + length - 1};
                    Range destination_range{destination_start, destination_start + length - 1};
                    mappings.push_back({source_range, destination_range});
                }
            }
            std::sort(mappings.begin(), mappings.end(), [](auto a, auto b) {
                return a.first.start < b.first.start;
            });
            Mapping mapping;
            int current_start = 0;
            for (int i = 0; i < mappings.size(); i++) {
                auto [source_range, destination_range] = mappings[i];
                if (source_range.start > current_start) {
                    auto source = Range{current_start, source_range.start - 1};
                    auto destination = Range{current_start, source_range.start - 1};
                    mapping.mappings.push_back({source, destination});
                }
                mapping.mappings.push_back({source_range, destination_range});
                current_start = source_range.end + 1;
            }
            almanac.push_back(mapping);
        }
    }
    return {almanac, seeds};
}


long find_min_location(Range range, std::vector<Mapping> almanac) {
    std::vector<Range> next;
    std::vector<Range> current{range};
    for (auto &mapping: almanac) {
        next.clear();
        for (auto &source: current) {
            auto result = mapping.apply(source);
            next.insert(next.end(), result.begin(), result.end());
        }
        current = next;
    }
    auto min = std::min_element(current.begin(), current.end(), [](auto a, auto b) {
        return a.start < b.start;
    });
    return min->start;
}

long task_1(std::vector<Mapping> mappings, std::vector<long> seeds) {
    std::vector<Range> ranges;
    for (int i = 0; i < seeds.size(); i++) {
        ranges.push_back({seeds[i], seeds[i]});
    }
    std::vector<long> locations;
    std::transform(ranges.begin(), ranges.end(), std::back_inserter(locations), [&](auto &range) {
        return find_min_location(range, mappings);
    });
    return *std::min_element(locations.begin(), locations.end());
}

long task_2(std::vector<Mapping> mappings, std::vector<long> seeds) {
    std::vector<Range> ranges;
    for (int i = 0; i < seeds.size(); i += 2) {
        ranges.push_back({seeds[i], seeds[i] + seeds[i + 1] - 1});
    }
    std::vector<long> locations;
    std::transform(ranges.begin(), ranges.end(), std::back_inserter(locations), [&](auto &range) {
        return find_min_location(range, mappings);
    });
    return *std::min_element(locations.begin(), locations.end());
}

int main() {
    std::string input = "input.txt";
    auto [mappings, seeds] = parse(input);
    std::cout << "Task 1: " << task_1(mappings, seeds) << std::endl;
    std::cout << "Task 2: " << task_2(mappings, seeds) << std::endl;
}
