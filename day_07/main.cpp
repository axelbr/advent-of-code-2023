#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <numeric>
#include <set>

enum Card {
    Two = 2,
    Three = 3,
    Four = 4,
    Five = 5,
    Six = 6,
    Seven = 7,
    Eight = 8,
    Nine = 9,
    Ten = 10,
    Jack = 11,
    Queen = 12,
    King = 13,
    Ace = 14
};

struct Hand {
    std::vector<Card> cards;
    bool weak_jack = false;

    bool operator<(const Hand &other) const {
        int rank = this->rank();
        int other_rank = other.rank();
        if (rank == other_rank) {
            for (int i = 0; i < this->cards.size(); i++) {
                if (this->cards[i] != other.cards[i]) {
                    if (this->weak_jack and this->cards[i] == Card::Jack) {
                        return true;
                    } else if (this->weak_jack and other.cards[i] == Card::Jack) {
                        return false;
                    }
                    return this->cards[i] < other.cards[i];
                }
            }
            return false;
        }
        return rank < other_rank;
    }

    int rank() const {
        if (this->weak_jack) {
            auto cards = this->cards;
            std::map<Card, int> counts;
            for (auto card: cards) {
                if (card != Card::Jack) {
                    counts[card]++;
                }
            }
            Card max_card = std::max_element(counts.begin(), counts.end(), [](auto &a, auto &b) {
                return a.second < b.second;
            })->first;
            std::replace(cards.begin(), cards.end(), Card::Jack, max_card);
            Hand new_hand{cards, false};
            return new_hand.rank();
        }

        if (this->is_five_of_a_kind(this->cards)) {
            return 6;
        } else if (this->is_four_of_a_kind(this->cards)) {
            return 5;
        } else if (this->is_full_house(this->cards)) {
            return 4;
        } else if (this->three_of_a_kind(this->cards)) {
            return 3;
        } else if (this->two_pairs(this->cards)) {
            return 2;
        } else if (this->one_pair(this->cards)) {
            return 1;
        } else {
            return 0;
        }
    }

    bool is_five_of_a_kind(const std::vector<Card> &cards) const {
        std::set<Card> unique_cards(cards.begin(), cards.end());
        return unique_cards.size() == 1;
    }

    bool is_four_of_a_kind(const std::vector<Card> &cards) const {
        std::set<Card> unique_cards(cards.begin(), cards.end());
        auto count = std::count(cards.begin(), cards.end(), cards[0]);
        return unique_cards.size() == 2 and (count == 4 or count == 1);
    }

    bool is_full_house(const std::vector<Card> &cards) const {
        std::set<Card> unique_cards(cards.begin(), cards.end());
        auto count = std::count(cards.begin(), cards.end(), cards[0]);
        return unique_cards.size() == 2 and (count == 3 or count == 2);
    }

    bool three_of_a_kind(const std::vector<Card> &cards) const {
        std::set<Card> unique_cards(cards.begin(), cards.end());
        return unique_cards.size() == 3 and std::all_of(unique_cards.begin(), unique_cards.end(), [&](auto card) {
            auto count = std::count(cards.begin(), cards.end(), card);
            return count == 3 or count == 1;
        });

    }

    bool two_pairs(const std::vector<Card> &cards) const {
        std::set<Card> unique_cards(cards.begin(), cards.end());
        return unique_cards.size() == 3 and std::all_of(unique_cards.begin(), unique_cards.end(), [&](auto card) {
            auto count = std::count(cards.begin(), cards.end(), card);
            return count == 2 or count == 1;
        });
    }

    bool one_pair(const std::vector<Card> &cards) const {
        std::set<Card> unique_cards(cards.begin(), cards.end());
        return unique_cards.size() == 4;
    }
};

std::vector<std::pair<Hand, int>> parse(const std::string &input, bool weak_jack = false) {
    std::ifstream file(input);
    std::string line;
    std::vector<std::pair<Hand, int>> result;
    auto parse_card = [](char c) {
        switch (c) {
            case '2':
                return Card::Two;
            case '3':
                return Card::Three;
            case '4':
                return Card::Four;
            case '5':
                return Card::Five;
            case '6':
                return Card::Six;
            case '7':
                return Card::Seven;
            case '8':
                return Card::Eight;
            case '9':
                return Card::Nine;
            case 'T':
                return Card::Ten;
            case 'J':
                return Card::Jack;
            case 'Q':
                return Card::Queen;
            case 'K':
                return Card::King;
            case 'A':
                return Card::Ace;
        }
        return Card::Two;
    };

    while (std::getline(file, line)) {
        auto cards = line.substr(0, line.find(" "));
        auto bet = std::stoi(line.substr(line.find(" ") + 1));
        Hand hand{{}, weak_jack};
        std::transform(cards.begin(), cards.end(), std::back_inserter(hand.cards), parse_card);
        result.push_back({hand, bet});
    }
    return result;
}

int solve(std::vector<std::pair<Hand, int>> hands_and_bets) {
    std::sort(hands_and_bets.begin(), hands_and_bets.end(), [](auto &a, auto &b) {
        return a.first < b.first;
    });
    int sum = 0;
    for (int i = 1; i <= hands_and_bets.size(); i++) {
        auto [hand, bet] = hands_and_bets[i - 1];
        sum += bet * i;
    }
    return sum;
}

int main() {
    std::string input = "input.txt";
    std::cout << "Task 1: " << solve(parse(input, false)) << std::endl;
    std::cout << "Task 2: " << solve(parse(input, true)) << std::endl;
}
