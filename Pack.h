#ifndef PACK_H
#define PACK_H
//
//
// Pack.h
//


#include "Card.h"
#include <array>
#include <string>
#include <vector>

class Pack {
public:
    // EFFECTS: Initializes the Pack to have all 52 cards
    Pack();
    
    // REQUIRES: input variable must be an integer
    // EFFECTS: Initializes the pack to have multiple decks
    Pack(int decks);

    // REQUIRES: cards remain in the Pack
    // EFFECTS: Returns the next card in the pack and increments the next index
    Card deal_one();
    
    // EFFECTS: returns size of the pack
    int get_size();

    // EFFECTS: Shuffles the Pack and resets the next index.  See the project
    // spec for a description of the shuffling procedure
    void shuffle();

    // EFFECTS: returns true if there are no more cards left in the pack
    bool empty() const;
    
    // MODIFIES: changes the count of the cards depending on the upcard
    void set_count(Card c);
    
    // EFFECTS: Returns the card count
    const int & get_count();
        
    // HELPER FUNCTION
    // Used under the set_count function
    int find_index_rank(const std::string &rank_in);

private:
    std::vector<Card> cards;
    int card_count = 0;
};

#endif
