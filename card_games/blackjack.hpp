#include <array>
#include <vector>
#include <iostream>
#include <string>
#include <numeric>
#include <utility>

#include "deck.hpp"

#pragma once

namespace game {

class BlackJack {
public:
    BlackJack(const cards::Deck &deck) : _deck(deck), _deckIndex(0) {}

    void play() {
        bool playing = true;

        std::cout << "Welcome to the game of BlackJack" << std::endl;
        burn();

        while (playing) {
new_game:
            deal();

            std::cout << "Dealer showing ";
            std::cout << "(" << _dealer[1].showRank() << " " << _dealer[1].showSuit() << ")" << std::endl;

            int hand = 0, split = 0, count = 0;
            while (true) {
                // use a lambda so we don't need complicated exit logic
                auto showHand = [&, hand]() {
                    std::cout << "You have ";
                    for (auto c : _player[hand])
                        std::cout << "(" << c.showRank() << " " << c.showSuit() << ")" << value(_player[hand]) << std::endl;
                };
                auto check_busted = [&, hand]() -> bool {
                    if (value(_player[hand]) > 21) {
                        std::cout << "YOU BUSTED!" << std::endl;
                        return true;
                    }
                    return false;
                };

                showHand();

                char choice;
                std::cout << "(H)it " << ((count == 0) ? "(D)ouble " : " ") << "or (S)tand? ";
                std::cin >> choice;
                if (std::tolower(choice) == 'h') {
                    hit(hand);
                    showHand();
                    if (check_busted())
                        goto new_game;
                }
                else if (count == 0 && std::tolower(choice) == 'd') {
                    hit(hand);
                    showHand();
                    if (check_busted())
                        goto new_game;
                    break;
                }
                else if (std::tolower(choice) == 's')
                    break;
                
                count++;
            }
            
            std::cout << "Dealer has ";
            for (auto c : _dealer)
                std::cout << "(" << c.showRank() << " " << c.showSuit() << ")";
        }
    }

protected:
    void burn() {
        // burn a card
        _deckIndex++;
    }

    void deal() {
        // deal two cards
        for (int i = 0; i < 2; i++) {
            _player[0].push_back(_deck[_deckIndex++]);
            _dealer.push_back(_deck[_deckIndex++]);
        }
    }

    void hit(int hand) {
        _player[hand].push_back(_deck[_deckIndex++]);
    }

    // blackjack card values
    std::pair<int, int> card_value(cards::Card c) {
        std::pair<int, int> val;

        switch (c._rank) {
            case cards::Rank::Ace:
                val = std::make_pair(1, 11);
                break;
            case cards::Rank::Two:
                val = std::make_pair(2, 0);
                break;
            case cards::Rank::Three:
                val = std::make_pair(3, 0);
                break;
            case cards::Rank::Four:
                val = std::make_pair(4, 0);
                break;
            case cards::Rank::Five:
                val = std::make_pair(5, 0);
                break;
            case cards::Rank::Six:
                val = std::make_pair(6, 0);
                break;
            case cards::Rank::Seven:
                val = std::make_pair(7, 0);
                break;
            case cards::Rank::Eight:
                val = std::make_pair(8, 0);
                break;
            case cards::Rank::Nine:
                val = std::make_pair(9, 0);
                break;
            case cards::Rank::Ten:
            case cards::Rank::Jack:
            case cards::Rank::Queen:
            case cards::Rank::King:
                val = std::make_pair(10, 0);
                break;
        }

        return val;
    }

    int value(std::vector<cards::Card> &hand) {
        auto cardSum = [&](cards::Card a, cards::Card b) -> int {
            return card_value(a).first + card_value(b).first;
        };
        
    }

private:
    const cards::Deck &_deck;
    int _deckIndex;

    std::array<std::vector<cards::Card>, 4> _player; // use array to handle splitting
    std::vector<cards::Card> _dealer;
};

}
