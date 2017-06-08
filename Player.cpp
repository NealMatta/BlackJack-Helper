//
//  Player.cpp
//
//

#include "Card.h"
#include "Pack.h"
#include "Player.h"

#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <iostream>
#include <algorithm>

using namespace std;



// MODIFIES: Player hand
// EFFECTS: Adds a card to the player's hand
void Player::add_card(const Card &c){
    hand.push_back(c);
    set_val_of_hand(c);
}

// MODIFIES: player's hand
// EFFECTS: Adds a card to the player's hand
void Player::hit(Pack &deck_in){
    add_card(deck_in.deal_one());
}

// EFFECTS: Checks to see whether or not the player is bust
bool Player::bust() const{
    if (get_val_of_hand() > 21){ return true; }
    
    return false;
}

// EFFECTS: Check's whether or not the player got a blackjack
bool Player::blackjack() const{
    if (get_val_of_hand() == 21){
        cout << "\nBlackjack!\n\n";
        return true;
    }
    return false;
}

// MODIFIES: Changes value of ace and total value of hand
// EFFECTS: changes the value of the ace depending on the total score
void Player::ace_option(int &choice_before){
    int choice_after = 0;
    int ace_count = 0;
        
    for (int i = 0; i < get_hand_size(); i++){
        if (hand[i].get_rank() == "A"){
            
            ace_count++;
            
            if (ace_count >= 2){ // If there is more than one ace
                if (get_val_of_hand() - hand[i].get_choice_before() >= 11){ // If 11 busts the hand
                    choice_after = 1;
                }
                
                else {
                    choice_after = 11;
                }
                
                set_val_of_hand_ace(hand[i].get_choice_before(), choice_after);
                hand[i].set_choice_before(choice_after);
                
            }
            
            
            if ((card_in_hand("10") || card_in_hand("J") || // Checks for blackjack
                 card_in_hand("Q") || card_in_hand("K")) &&
                get_hand_size() == 2){
                
                set_val_of_hand_ace(0, 11);
                return;
            }
            
            else if (get_val_of_hand() - choice_before >= 11){ // If 11 busts the hand
                choice_after = 1;
            }
            
            else {
                choice_after = 11;
            }
            
            set_val_of_hand_ace(choice_before, choice_after);
            choice_before = choice_after;
            
        }
    }
}

// Helper Function
// EFFECTS: Iterates through the player's hand and returns true if a card ia found
bool Player::card_in_hand(std::string rank) const{
    for (int i = 0; i < get_hand_size(); i++){
        if(hand[i].get_rank() == rank){
            return true;
        }
    }
    return false;
}

// MODIFIES: hand
// EFFECTS: Splits the hand into two parts
vector<Card> Player::split_hand(){
    vector<Card> temp;
    
    Card removing = hand[get_hand_size() - 1];
    temp.push_back(removing);
    
    value_of_hand -= removing.rank_by_int();
    hand.pop_back();
    
    return temp;
}

// EFFECTS: Checks to see that there are two identical valued cards
bool Player::same_cards() {
    if (get_hand_size() == 2 && (hand[0].rank_by_int() == hand[1].rank_by_int())){
        return true;
    }
    return false;
}

// EFFECTS: Prints hand of the player such as:
//            hand: 2 J OR
//            hand: ** Q
void Player::print_hand(bool dealer_initial_hand) const {
    cout << "hand: ";
    
    if (dealer_initial_hand){ // Only true after human is all done playing
        cout << "** " << hand[1]; // hand: ** 2
    }
    
    else{
        for (int i = 0; i < hand.size(); i++){
            cout << hand[i] << " "; // hand: 2 6
        }
    }
}

// MODIFIES: value_of_hand
// EFFECTS: increases value of hand accordingly
void Player::set_val_of_hand(const Card &c){
    
    if (c.get_rank() == "2"){ value_of_hand += 2;}
    else if (c.get_rank() == "3"){ value_of_hand += 3;}
    else if (c.get_rank() == "4") { value_of_hand += 4;}
    else if (c.get_rank() == "5"){ value_of_hand += 5;}
    else if (c.get_rank() == "6") { value_of_hand += 6;}
    else if (c.get_rank() == "7"){ value_of_hand += 7;}
    else if (c.get_rank() == "8") { value_of_hand += 8;}
    else if (c.get_rank() == "9"){ value_of_hand += 9;}
    else if (c.get_rank() == "10" || c.get_rank() == "J" || c.get_rank() == "Q" || c.get_rank() == "K") { value_of_hand += 10;}

}

// MODIFIES: value_of_hand
// EFFECTS: increases value of hand for ace
void Player::set_val_of_hand_ace(const int ace_before, const int ace_after){
    /* Since player can choose the value of ace, and also change 
     their decision, their old choice must be subtracted from the
     value_of_hand
     */
    
    value_of_hand -= ace_before;
    value_of_hand += ace_after;
}

// EFFECTS: Returns value_of_hand
int Player::get_val_of_hand() const{
    return value_of_hand;
}

// EFFECTS: returns size of hand
int Player::get_hand_size() const{
    return hand.size();
}

















