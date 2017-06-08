//
//
// Pack.cpp
//


#include "Card.h"
#include "Pack.h"
#include <array>
#include <string>
#include <algorithm>
#include <cassert>
#include <stdlib.h>     // rand


using namespace std;


Pack::Pack() {
    for (int s = 0; s < 4; s++){ // Makes a full deck into a vector
        for (int n = 0; n < 13; n++){
            Card temp(RANK_NAMES_BY_WEIGHT[n]);
            cards.push_back(temp);
        }
    }
}

// REQUIRES: input variable must be an integer
// EFFECTS: Initializes the pack to have multiple decks
Pack::Pack(int decks) {
    assert(decks >= 1);
   
    for (int i = 0; i < decks; i++){
        for (int s = 0; s < 4; s++){ // Suits
            for (int n = 0; n < 13; n++){ // Values
                Card temp(RANK_NAMES_BY_WEIGHT[n]);
                cards.push_back(temp);
            }
        }
    }
}

// REQUIRES: cards remain in the Pack
// EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one() {
    assert(!empty());
    
    Card temp = cards.back();
    set_count(temp);
    
    cards.pop_back();
    return temp;
}

// EFFECTS: returns size of the pack
int Pack::get_size(){
    return cards.size();
}

// EFFECTS: Returns the card count
const int & Pack::get_count(){
    return card_count;
}

// MODIFIES: changes the count of the cards depending on the upcard
void Pack::set_count(Card c){
    
    if (find_index_rank(c.get_rank()) >= find_index_rank("2") && find_index_rank(c.get_rank()) <= find_index_rank("6")){
        // Cards 2-6 get assigned +1 value
        card_count++;
    }
    
    else if (find_index_rank(c.get_rank()) >= find_index_rank("10") && find_index_rank(c.get_rank()) <= find_index_rank("A")){
        // Cards 10-A get assigned -1 value
        card_count--;
    }
    
    else{
        // Cards 7-9 get assigned a 0 value
        card_count += 0;
    }
}

// HELPER FUNCTION
// Used under the set_count function
int Pack::find_index_rank(const string &rank_in) {
    for (int i = 0; i < NUM_RANKS; i++) {
        if (RANK_NAMES_BY_WEIGHT[i] == rank_in) {
            return i;
        }
    }
    return 0;
}

// EFFECTS: Shuffles the Pack and resets the next index.  See the project
// spec for a description of the shuffling procedure
void Pack::shuffle() {
    random_shuffle(cards.begin(), cards.end());

}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const {
    if (cards.size() == 0) { return true;}
    return false;
}
