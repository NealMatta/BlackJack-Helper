//
//
// Card.cpp
//

#include <iostream>
#include <string>
#include <cassert>
#include "Card.h"


using namespace std;


int find_index_rank(const string &rank_in);
int find_index_suit(const string &suit_in);

// REQUIRES: rank is one of "Two", "Three", "Four", "Five", "Six", "Seven",
//  "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"
// EFFECTS: Initializes Card to specified rank and suit
Card::Card(const string &rank_in) : rank(rank_in) {}

// EFFECTS: Returns the rank
string Card::get_rank() const { return rank; }


// EFFECTS: Prints Card to stream, for example "Two of Spades"
std::ostream & operator<<(std::ostream &os, const Card &card) {
    return os << card.get_rank();
}


// EFFECTS: Determines value of all cards except Ace
int Card::rank_by_int() const{
    if (rank == "10" || rank == "J" || rank == "Q" || rank == "K") { return 10;}
    else if( rank == "A") { return 0; }
    else { return stoi(rank); }
    
}

//EFFECTS: Finds the index of the rank input
int find_index_rank(const string &rank_in) {
    for (int i = 0; i < NUM_RANKS; i++) {
        if (RANK_NAMES_BY_WEIGHT[i] == rank_in) {
            return i;
        }
    }
    return 0;
}

// MODIFIES: choice_before_card
// EFFECTS: Set's ace_value_per_card t0 given integer value
void Card::set_choice_before(int one_or_eleven){
    ace_value_per_card = one_or_eleven;
}

// EFFECTS: Return's ace_value_per_card
int Card::get_choice_before(){
    return ace_value_per_card;
}
