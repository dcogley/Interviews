#include <array>
#include <string>
#include <string_view>

#pragma once

namespace cards {

enum class Suit {
    Hearts,
    Diamonds,
    Clubs,
    Spades
};

enum class Rank {
    Ace,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};

struct Card {
    Suit _suit;
    Rank _rank;

    std::string showRank() {
        std::string r;
        switch (_rank) {
            case Rank::Ace:
                r += "Ace";
            break;
            case Rank::Two:
                r += "Two";
            break;
            case Rank::Three:
                r += "Three";
            break;
            case Rank::Four:
                r += "Four";
            break;
            case Rank::Five:
                r += "Five";
            break;
            case Rank::Six:
                r += "Six";
            break;
            case Rank::Seven:
                r += "Seven";
            break;
            case Rank::Eight:
                r += "Eight";
            break;
            case Rank::Nine:
                r += "Nine";
            break;
            case Rank::Ten:
                r += "Ten";
            break;
            case Rank::Jack:
                r += "Jack";
            break;
            case Rank::Queen:
                r += "Queen";
            break;
            case Rank::King:
                r += "Kind";
            break;
        }
        return r;
    }

    std::string showSuit() {
        std::string s("of ");
        switch (_suit) {
            case Suit::Hearts:
                s += "Hearts";
            break;
            case Suit::Diamonds:
                s += "Diamonds";
            break;
            case Suit::Clubs:
                s += "Clubs";
            break;
            case Suit::Spades:
                s += "Spades";
            break;
        }
        return s;
    }

};

// standard deck of cards is 52 cards
constexpr int NUMBER_CARDS = 52;
// use compile time as the seed
constexpr std::string_view str = __TIME__;
struct Deck : public std::array<Card, NUMBER_CARDS> {
    
    uint32_t _seed; // rng seed

    constexpr Deck() : 
        std::array<Card, NUMBER_CARDS>(),
        _seed(fnv1a_hash(str) * 1664525UL + 1013904223UL) {
    }
    
    constexpr uint32_t fnv1a_hash(std::string_view str) {
        uint32_t kFnvPrime = 16777619u;
        uint32_t kFnvOffset = 2166136261u;
        uint32_t hash = kFnvOffset;
        for (char c : str) {
            hash ^= static_cast<uint32_t>(c);
            hash *= kFnvPrime;
        }
        return hash;
    }
    
    constexpr uint32_t random() {
        _seed = _seed * 1664525UL + 1013904223UL;
        return _seed;
    }

    // do a Fisher-Yates shuffle
    constexpr void shuffle(unsigned int u = 0) {
        if (u + 1 < size()) {
            uint32_t rnd = random() % size();
            Card c = at(u);
            at(u) = at(rnd);
            at(rnd) = c;
            shuffle(u + 1);
        }
    }
};

template<typename Iter, typename Enum>
constexpr Iter make_iter(Enum e) {
    return static_cast<Iter>(e);
}

template<typename Enum, typename Iter>
constexpr Enum make_enum(Iter i) {
    return static_cast<Enum>(i);
}

constexpr Deck create_deck(bool shuffle) {
    
    Deck deck{};
    int idx = 0;
    
    // we get fancy here and use type inference
    for (auto suit = make_iter<int>(Suit::Hearts); suit <= make_iter<int>(Suit::Spades); suit++) {
        for (auto rank = make_iter<int>(Rank::Ace); rank <= make_iter<int>(Rank::King); rank++) {
            deck[idx++] = { make_enum<Suit>(suit), make_enum<Rank>(rank) };
        }
    }

    // shuffle the deck
    if (shuffle)
        deck.shuffle();

    return deck;
}

}
