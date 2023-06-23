#include <iostream>

#include "blackjack.hpp"

int main() {
    // create/shuffle the deck at compile time
    constexpr auto deck = cards::create_deck(true);

    // lets play blackjack!
    game::BlackJack bj(deck);
    bj.play();

    return 0;
}
