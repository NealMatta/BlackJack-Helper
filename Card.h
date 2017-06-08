#ifndef CARD_H
#define CARD_H
//
//
// Card.cpp
//

#include <iostream>
#include <string>

class Card {
public:
  // rank and suit names
  static constexpr const char* const RANK_TWO = "2";
  static constexpr const char* const RANK_THREE = "3";
  static constexpr const char* const RANK_FOUR = "4";
  static constexpr const char* const RANK_FIVE = "5";
  static constexpr const char* const RANK_SIX = "6";
  static constexpr const char* const RANK_SEVEN = "7";
  static constexpr const char* const RANK_EIGHT = "8";
  static constexpr const char* const RANK_NINE = "9";
  static constexpr const char* const RANK_TEN = "10";
  static constexpr const char* const RANK_JACK = "J";
  static constexpr const char* const RANK_QUEEN = "Q";
  static constexpr const char* const RANK_KING = "K";
  static constexpr const char* const RANK_ACE = "A";

    

    // REQUIRES: rank is one of "Two", "Three", "Four", "Five", "Six", "Seven",
    //  "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"
    // EFFECTS Initializes Card to specified rank
    Card(const std::string &rank_in);

    // EFFECTS: Returns the rank
    std::string get_rank() const;

    // EFFECTS: Determines value of all cards except Ace
    int rank_by_int() const;
    
    // MODIFIES: choice_before_card
    // EFFECTS: Set's ace_value_per_card t0 given integer value
    void set_choice_before(int one_or_eleven);
    
    // EFFECTS: Return's ace_value_per_card
    int get_choice_before();

private:
    std::string rank;
    int ace_value_per_card = 0;
};

// Ranks in order from lowest rank to highest rank.
static const std::string RANK_NAMES_BY_WEIGHT[] = {
    Card::RANK_TWO,
    Card::RANK_THREE,
    Card::RANK_FOUR,
    Card::RANK_FIVE,
    Card::RANK_SIX,
    Card::RANK_SEVEN,
    Card::RANK_EIGHT,
    Card::RANK_NINE,
    Card::RANK_TEN,
    Card::RANK_JACK,
    Card::RANK_QUEEN,
    Card::RANK_KING,
    Card::RANK_ACE
};
static const int NUM_RANKS = 13;


// EFFECTS: Prints Card to stream, for example "Two of Spades"
std::ostream & operator<<(std::ostream &os, const Card &card);



#endif
