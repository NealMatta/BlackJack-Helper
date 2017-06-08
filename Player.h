#ifndef PLAYER_H
#define PLAYER_H
//
//  Player.h
//
//


#include "Card.h"
#include "Pack.h"
#include <string>
#include <vector>




class Player {
    
public:
    
    // MODIFIES: Player hand
    // EFFECTS: Adds a card to the player's hand
    void add_card(const Card &c);
    
    // MODIFIES: player's hand
    // EFFECTS: Adds a card to the player's hand
    void hit(Pack &deck_in);
    
    // EFFECTS: Checks to see whether or not the player is bust
    bool bust() const;

    // EFFECTS: Check's whether or not the player got a blackjack
    bool blackjack() const;

    // EFFECTS: Prints hand of the player such as:
    //            hand: 2 J OR
    //            hand: ** Q
    void print_hand(bool dealer_initial_hand) const;
    
    // MODIFIES: value_of_hand
    // EFFECTS: increases value of hand accordingly
    void set_val_of_hand(const Card &c);
    
    // MODIFIES: value_of_hand
    // EFFECTS: increases value of hand for ace
    void set_val_of_hand_ace(const int ace_before, const int ace_after);

    // EFFECTS: Returns value_of_hand
    int get_val_of_hand() const;
    
    // EFFECTS: returns size of hand
    int get_hand_size() const;
    
    // MODIFIES: Changes value of ace and total value of hand
    // EFFECTS: changes the value of the ace depending on user input
    void ace_option(int &choice_before);
    
    // Helper Function
    // EFFECTS: Iterates through the player's hand and returns true if a card ia found
    bool card_in_hand(std::string rank) const;
    
    // MODIFIES: hand
    // EFFECTS: Splits the hand into two parts
    std::vector<Card> split_hand();
    
    // EFFECTS: Checks to see that there are two identical valued cards
    bool same_cards();

    // EFFECTS: Basic strategy help for the user if needed
    friend void basic_strategy(const Player &d_in, const Player &h_in, int value_of_ace);
    
    // MODIFIES: Humans' hands (Initial hand and split hand)
    // EFFECTS: Human splits hand and plays both
    friend void split(const Player &d_in, Player &h_in, Pack &deck_in, int &ace_before_human, bool &split_function_called, Player &first_hand_for_human);
private:
    std::vector<Card> hand;
    int value_of_hand = 0;
};
#endif
