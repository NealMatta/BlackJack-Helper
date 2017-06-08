//
//  gameplay.cpp
//  BlackJack Helper
//
//  Created by Neal Matta on 5/24/17.
//


#include "Card.h"
#include "Pack.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>  // exit, EXIT_FAILURE
#include <algorithm> // transform

using namespace std;

void intro_text();
void menu();
void deal_cards(Player &d, Player &h, Pack &deck_in);
void starting_game(Player &d_in, Player &h_in, Pack &deck_in, int &round_count);
void print_hands(const Player &d, const Player &h, bool initial_round, bool reveal_score);
void reveal_score(bool final_round, const Player &p_in);
void gameplay_human(const Player &d_in, Player &h_in, Pack &deck_in, int &ace_before_human, bool &split_function_called, Player &first_hand_split);
void automate_dealer(Player &d_in, const Player &h_in, Pack &deck_in, int &ace_before_dealer);
void compare(const Player &d_in, const Player &h_in);
void basic_strategy(const Player &d_in, const Player &h_in, int value_of_ace);

Pack setting_deck();

const string HIT = "H";
const string STAND = "ST";
const string SPLIT = "SP";
const string DD = "DD";
const string QUIT = "Q";
const string COUNT = "C";
const string MENU = "M";
const string BASIC_STRATEGY = "BS";

int main(){
    intro_text();
    
    Pack deck = setting_deck();
    int round = 1;
    
    while (deck.get_size() > 3){
        Player first_hand_for_human; // Second hand used for the split function
        bool split_called = false;
        int set_value_of_ace_human = 0;
        int set_value_of_ace_dealer = 0;

        Player dealer, human;
        
        starting_game(dealer, human, deck, round);
        
        gameplay_human(dealer, human, deck, set_value_of_ace_human, split_called, first_hand_for_human);
        
        if (split_called){ // If the split function is called
        
            if((human.bust() && !first_hand_for_human.bust()) || // If first hand busted and second did NOT ...
               (!human.bust() && !first_hand_for_human.bust())){ // ... if neither busted, pass either one through
                automate_dealer(dealer, first_hand_for_human, deck, set_value_of_ace_dealer);
            }
            
            else if(!human.bust() && first_hand_for_human.bust()){ // If first hand did NOT bust and second hand did
                automate_dealer(dealer, human, deck, set_value_of_ace_dealer);
            }
            
            // Prints out the comparisons for both hands
            cout << "\nFIRST HAND OUTCOME\n";
            compare(dealer, first_hand_for_human);
            
            cout << "\nSECOND HAND OUTCOME\n";
            compare(dealer, human);
        }

        else{ // If split function is not called
            automate_dealer(dealer, human, deck, set_value_of_ace_dealer);
            compare(dealer, human);
        }
    
        cout << endl;
    }
    
    cout << "There are no more cards to play with\n";
}

// EEFECTS: Prints intro text describing the game and the way it's played
void intro_text(){
    cout << "|||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Blackjack Helper ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|||" << endl;
    cout << "|||~~~ Counts cards and provides option for correct choice (with basic strategy) ~~~|||" << endl;
    cout << "|||~~~~~~~~~~~~~~~~~~~~~ Dealer must fold on a soft 17 (A + 6) ~~~~~~~~~~~~~~~~~~~~~|||\n" << endl;

    
}

// EFFECTS: prints menu for allowed responses
void menu(){
    cout << "|| ~~~~~~~~~~~~~~~~~ MENU ~~~~~~~~~~~~~~~~~~ || \n"
         << "|| ~~~~~~~~~~ 1. enter \"H\" to hit ~~~~~~~~~~ ||\n"
         << "|| ~~~~~~~~~~ 2. enter \"ST\" to stand ~~~~~~~ ||\n"
         << "|| ~~~~~~~~~~ 3. enter \"SP\" to split ~~~~~~~ ||\n"
         << "|| ~~~~~~~~~~ 4. enter \"D\" to double down ~~ ||\n"
         << "|| ~~~~~~~~~~ 5. enter \"C\" to check count ~~ ||\n"
         << "|| ~~~~~~~~~~ 6. enter \"BS\" for strategy ~~~ ||\n"
         << "|| ~~~~~~~~~~ 7. enter \"M\" for the menu ~~~~ ||\n"
         << "|| ~~~~~~~~~~ 8. enter \"Q\" to quit ~~~~~~~~~ ||\n\n";
}

// REQUIRES: Number of decks must be an integer greater than 0
// EFFECTS: Initializes and returns the deck that will be played with and shuffles it a random number of times
Pack setting_deck(){
    
    srand(time(NULL)); // Ensures that there is a different random number every time
    
    int num_decks = 0;
    int random_number = rand()%50 + 1;
    
    cout << "How many decks you would like to play with: ";
    cin >> num_decks;
    
    menu();  // Prints the menu
    
    Pack playing_with(num_decks); // Initializes 'x' number of decks into 1
    
    for (int i = 0; i < random_number; i++){ // Shuffles the deck a random number of times
        playing_with.shuffle();
    }
    
    cout << endl;
    
    return playing_with;

}

// MODIFIES: Players' hands
// EFFECTS: Dealer and human now have two cards to start with
void deal_cards(Player &d, Player &h, Pack &deck_in){
    for (int i = 0; i < 2; i++){
        d.add_card(deck_in.deal_one());
        h.add_card(deck_in.deal_one());
    }
    
    
}

// MODIFIES: round number
// EFFECTS: Initializes the game (prints the round, deals the cards, prints hands)
void starting_game(Player &d_in, Player &h_in, Pack &deck_in, int &round_count){
    cout << "Round " << round_count << endl << endl; // Prints out round numbers
        
    deal_cards(d_in, h_in, deck_in);
    print_hands(d_in, h_in, true, false);
    
    round_count++; // Increases the round count for the next hand
}

// EFFECTS: Prints both players hand
void print_hands(const Player &d, const Player &h, bool initial_round, bool final_round) {
    // Printing in the initial round only shows one card in the dealer's hand
    // Printing in the final round shows the hand count of both cards
    
    cout << "Dealer's ";
    d.print_hand(initial_round);
    
    reveal_score(final_round, d);
    
    cout << "Human's ";
    h.print_hand(false);
    
    reveal_score(final_round, h);
}

// Helper function for "print_hands" function
// EFFECTS: Prints out the hand's total value
void reveal_score(bool final_round, const Player &p_in){
    if (final_round){ cout << "(" << p_in.get_val_of_hand() << ")"; }
    cout << endl;
}

// MODIFIES: Humans' hands (Initial hand and split hand)
// EFFECTS: Human splits hand and plays both
void split(const Player &d_in, Player &h_in, Pack &deck_in, int &ace_before_human, bool &split_function_called, Player &first_hand_for_human){
    vector<string> hand_names = {"\n1ST HAND ", "\n2ND HAND "};
    
    vector<Card> second_hand = h_in.split_hand(); // Split hand contains the second card from the original hand
    
    // Prints the dealer's hand and the human's with the split hand
    print_hands(d_in, h_in, true, false);
    cout << "Split Hand: " << second_hand[0] << endl;
    
    for (int i = 0; i < 2; i++){
        // Plays a round with each of the human's cards
        cout << hand_names[i] << endl;
        print_hands(d_in, h_in, true, false);
        gameplay_human(d_in, h_in, deck_in, ace_before_human, split_function_called, first_hand_for_human);
        print_hands(d_in, h_in, true, false);
        cout << hand_names[i] << "COMPLETE\n";
        
        if (i == 1) {break;} // Only want to go through the process one time
        
        first_hand_for_human = h_in; // Stores the complete hand
        
        // Resets the hand for the new split hand
        h_in.value_of_hand = 0;
        h_in.set_val_of_hand(second_hand[0].get_rank());
        // Copies over the split hand
        h_in.hand = second_hand;
        
        cout << endl;
        
    }
}

// MODIFIES: Human's hand (normally as well as if split occurs), deck
// EFFECTS: Allows the human to choose how to play the game
void gameplay_human(const Player &d_in, Player &h_in, Pack &deck_in, int &ace_before_human, bool &split_function_called, Player &first_hand_split){
    string response;
    
    h_in.ace_option(ace_before_human); // Automates ace in human's hand
    
    if (h_in.blackjack()) { return; }
    
    cout << "\nChoice: ";
    cin >> response;
    cout << endl;
    
    transform(response.begin(), response.end(), response.begin(), ::toupper); // makes the response non case sensitive
    
    while (response != STAND){
        
        if (response == HIT){
            h_in.hit(deck_in);
            
            h_in.ace_option(ace_before_human); // Changes value of Ace if neccesary
            
            if (h_in.bust()){ break; }
            
            print_hands(d_in, h_in, true, false);
        }
        
        else if (response == SPLIT){
            if (h_in.same_cards()){
                split_function_called = true;
                split(d_in, h_in, deck_in, ace_before_human, split_function_called, first_hand_split);
                break;
            }
            cout << "Split is only allowed when cards have the same value (3 3, J 10, etc.)\n";
            
        }
        
        else if(response == DD){
            if (h_in.get_hand_size() == 2){
                cout << "Double Down\n";
                h_in.hit(deck_in);
                break;
            }
            cout << "Double down is not allowed\n";
        }
        
        else if (response == COUNT){
            cout << "Count: " << deck_in.get_count() << endl;
        }
        
        else if (response == MENU){
            menu();
        }
        
        else if (response == QUIT){
            cout << "You quit the game" << endl;
            exit(0);
        }
        
        else if (response == BASIC_STRATEGY){
            basic_strategy(d_in, h_in, ace_before_human);
        }
        
        else{
            cout << "That is not an appropriate response\n";
        }
        
        cout << "\nChoose again: ";
        cin >> response;
        cout << endl;
        
        transform(response.begin(), response.end(), response.begin(), ::toupper); // makes the response non case sensitive
    }
    
}

// MODIFIES: Dealer's hand
// EFFECTS: Dealer plays the hand as long as the human isn't bust or he has a score less than 17
void automate_dealer(Player &d_in, const Player &h_in, Pack &deck_in, int &ace_before_dealer){
    d_in.ace_option(ace_before_dealer); // Used to determine what to do with the dealer's ace
    
    while (d_in.get_val_of_hand() < 17 && !h_in.bust()){
        d_in.hit(deck_in);
        d_in.ace_option(ace_before_dealer); // Used to determine what to do with the dealer's ace
    }
}

// EFFECTS: Basic strategy help for the user if needed
void basic_strategy(const Player &d_in, const Player &h_in, int value_of_ace){
    
    cout << "BASIC STRATEGY\n";
    
    //                              Splits
    // Can only split when your first two cards are a pair
    
    if (h_in.hand[0].get_rank() == h_in.hand[1].get_rank() && h_in.get_hand_size() == 2){
        // If first card and second card have same values and this is the inital round
        
        cout << "SPLIT OPPORTURNITY\n";
        
        if (h_in.card_in_hand("A")){
            // Always split aces
            cout << "Correct choice: Split\nReason: Always split aces \n";
        }
        else if (h_in.card_in_hand("K") || h_in.card_in_hand("Q") ||
                 h_in.card_in_hand("J") || h_in.card_in_hand("10")){
            // Never split tens
            cout << "Correct choice: Stand\nReason: Never split tens \n";
        }
        else if (h_in.card_in_hand("9")){
            // A pair of 9’s splits against dealer 2 through 9, except for 7, otherwise stand
            if (d_in.hand[1].get_rank() == "2" || d_in.hand[1].get_rank() == "3" ||
                d_in.hand[1].get_rank() == "4" || d_in.hand[1].get_rank() == "5" ||
                d_in.hand[1].get_rank() == "6" || d_in.hand[1].get_rank() == "8" ||
                d_in.hand[1].get_rank() == "9"){
                cout << "Correct choice: Split\nReason: A pair of 9’s splits against dealer 2 - 9, except for dealer 7 \n";
            }
            
            else { cout << "Correct choice: Stand\nReason: A pair of 9’s stands against dealer 7 or dealer 10 - K \n"; }
        }
        else if (h_in.card_in_hand("8")){
            // Always split 8’s
            cout << "Correct choice: Split\nReason: Always split 8’s\n";
        }
        else if (h_in.card_in_hand("7") || h_in.card_in_hand("3") || h_in.card_in_hand("2")){
            // A pair of 7’s splits against dealer 2 through 7, otherwise hit
            // A pair of 3’s splits against dealer 2 through 7, otherwise hit
            // A pair of 2’s splits against dealer 2 through 7, otherwise hit
            
            if (d_in.hand[1].get_rank() == "2" || d_in.hand[1].get_rank() == "3" ||
                d_in.hand[1].get_rank() == "4" || d_in.hand[1].get_rank() == "5" ||
                d_in.hand[1].get_rank() == "6" || d_in.hand[1].get_rank() == "7"){
                cout << "Correct choice: Split \nReason: A pair of 2's, 3's, and 7’s splits against dealer 2 - 7\n";
            }
            else { cout << "Correct choice: Hit \nReason: A pair of 2's, 3's, and 7’s hits against dealer A or dealer 8 - K \n"; }
        }
        else if (h_in.card_in_hand("6")){
            // A pair of 6’s splits against dealer 2 through 6, otherwise hit
            if(d_in.hand[1].get_rank() == "2" || d_in.hand[1].get_rank() == "3" ||
               d_in.hand[1].get_rank() == "4" || d_in.hand[1].get_rank() == "5" ||
               d_in.hand[1].get_rank() == "6"){
                cout << "Correct choice: Split \nReason: A pair of 6’s splits against dealer 2 - 6 \n";
            }
            else {cout << "Correct choice: Hit \nReason: A pair of 6’s hits against dealer A or dealer 7 - K \n"; }
        }
        else if (h_in.card_in_hand("5")){
            // A pair of 5’s doubles against dealer 2 through 9, otherwise hit
            if (d_in.hand[1].get_rank() == "2" || d_in.hand[1].get_rank() == "3" ||
                d_in.hand[1].get_rank() == "4" || d_in.hand[1].get_rank() == "5" ||
                d_in.hand[1].get_rank() == "6" || d_in.hand[1].get_rank() == "7" ||
                d_in.hand[1].get_rank() == "7" || d_in.hand[1].get_rank() == "9"){
                cout << "Correct choice: Double Down \nReason: A pair of 5’s doubles against dealer 2 - 9 \n";
            }
            else {cout << "Correct choice: Hit \nReason: A pair of 5’s hits against dealer A or dealer 10 - K \n"; }
        }
        else if (h_in.card_in_hand("4")){
            // A pair of 4’s splits against dealer 5 and 6, otherwise hit
            if (d_in.hand[1].get_rank() == "5" || d_in.hand[1].get_rank() == "6"){
                cout << "Correct choice: Split \nReason: A pair of 4’s splits against dealer 5 and 6\n";
            }
            else {cout << "Correct choice: Hit \nReason: A pair of 4’s hits against dealer A - 4 or dealer 7 - K \n"; }
        }
    }
    
    //                              Soft Totals
    // A soft total is any hand that has an Ace as one of the first two cards, the ace counts as 11 to start.
    
    else if (h_in.card_in_hand("A") && (h_in.get_val_of_hand() - value_of_ace < 10)){
        // If there is an Ace
        
        cout << "SOFT TOTAL\n";
        if (h_in.card_in_hand("9")){
            // Soft 20 (A,9) always stands
            cout << "Correct choice: Stand \nReason: Soft 20 (A,9) always stands\n";
        }
        
        else if (h_in.card_in_hand("8")){
            // Soft 19 (A,8) doubles against dealer 6, otherwise stand.
            if (d_in.hand[1].get_rank() == "6"){ cout << "Double Down \nSoft 19 (A,8) doubles against dealer 6\n"; }
            else{ cout << "Correct choice: Stand \nReason: Soft 19 (A,8) doubles against dealer 6, otherwise stand \n"; }
        }
        
        else if (h_in.card_in_hand("7")){
            // Soft 18 (A,7) doubles against dealer 2 through 6, and hits against 9 through Ace, otherwise stand.
            if (d_in.hand[1].get_rank() == "2" || d_in.hand[1].get_rank() == "3" ||
                d_in.hand[1].get_rank() == "4" || d_in.hand[1].get_rank() == "5" ||
                d_in.hand[1].get_rank() == "6"){
                cout << "Correct choice: Double Down \nReason: Soft 18 (A,7) doubles against dealer 2 - 6\n";
            }
            else if (d_in.hand[1].get_rank() == "9" || d_in.hand[1].get_rank() == "10"||
                     d_in.hand[1].get_rank() == "J" || d_in.hand[1].get_rank() == "Q" ||
                     d_in.hand[1].get_rank() == "K"){
                cout << "Correct choice: Hit \nReason: Soft 18 (A,7) hits against dealer 9 - A\n" << endl;
            }
            else { cout << "Correct choice: Stand \nReason: Soft 18 (A,7) stands against dealer 7 - 8\n"; }
        }
        
        else if (h_in.card_in_hand("6")){
            // Soft 17 (A,6) doubles against dealer 3 through 6, otherwise hit.
            if (d_in.hand[1].get_rank() == "3" || d_in.hand[1].get_rank() == "4" ||
                d_in.hand[1].get_rank() == "5" || d_in.hand[1].get_rank() == "6"){
                cout << "Correct choice: Double Down \nReason: Soft 17 (A,6) doubles against dealer 3 - 6\n";
            }
            
            else { cout << "Correct choice: Hit \nReason: Soft 17 (A,6) hits against dealer A - 2 and 7 - K\n"; }
        }
        
        else if (h_in.card_in_hand("5") || h_in.card_in_hand("4")){
            // Soft 16 (A,5) and soft 15 (A,4) doubles against dealer 4 through 6, otherwise hit.
            
            if (d_in.hand[1].get_rank() == "4" || d_in.hand[1].get_rank() == "5" ||
                d_in.hand[1].get_rank() == "6"){
                cout << "Correct choice: Double Down \nReason: Soft 15 (A,4) and soft 16 (A,5) doubles against dealer 4 - 6 \n";
            }
            
            else { cout << "Correct choice: Hit \nReason: Soft 15 (A,4) and soft 16 (A,5) hits against dealer A - 3 and dealer 7 - K \n"; }
        }
        
        else if (h_in.card_in_hand("3") || h_in.card_in_hand("2")){
            // Soft 14 (A,3) and soft 13 (A,2) doubles against dealer 5 through 6, otherwise hit.
            
            if (d_in.hand[1].get_rank() == "5" || d_in.hand[1].get_rank() == "6"){
                cout << "Correct choice: Double Down \nReason: Soft 13 (A,2) and soft 14 (A,3) doubles against dealer 5 - 6 \n";
            }
            
            else { cout << "Correct choice: Hit \nReason: Soft 13 (A,2) and soft 14 (A,3) hits against dealer A - 4 and dealer 7 - K \n"; }
        }
    }
    
    //                              Hard Totals
    // A hard total is any hand that does not start with an ace in it, or it has been dealt an ace that can only be counted as 1 instead of 11
    
    else {
        cout << "HARD TOTAL\n";
        if (h_in.get_val_of_hand() >= 17){
            // 17 and up always stands.
            
            cout << "Correct choice: Stand \nReason: 17 and up always stands \n";
        }
        else if (h_in.get_val_of_hand() == 16 || h_in.get_val_of_hand() == 15 ||
                 h_in.get_val_of_hand() == 14 || h_in.get_val_of_hand() == 13){
            // 13, 14, 15, 16 stands against dealer 2 through 6, otherwise hit.
            if (d_in.hand[1].get_rank() == "2" || d_in.hand[1].get_rank() == "3" ||
                d_in.hand[1].get_rank() == "4" || d_in.hand[1].get_rank() == "5" ||
                d_in.hand[1].get_rank() == "6"){
                cout << "Correct choice: Stand \nReason: 13, 14, 15, and 16 stands against dealer 2 - 6 \n";
            }
            
            else { cout << "Correct choice: Hit \nReason: 13, 14, 15, 16 hit against dealer A and dealer 7 - K \n"; }
        }
        else if (h_in.get_val_of_hand() == 12){
            // 12 stands against dealer 4 through 6, otherwise hit.
            if (d_in.hand[1].get_rank() == "4" || d_in.hand[1].get_rank() == "5" ||
                d_in.hand[1].get_rank() == "6"){
                cout << "Correct choice: Stand \nReason: 12 stands against dealer 4 - 6";
            }
            
            else { cout << "Correct choice: Hit \nReason: 12 hit against dealer A - 3 and dealer 7 - K \n"; }
        }
        else if (h_in.get_val_of_hand() == 11){
            // 11 always doubles.
            cout << "Correct choice: Double Down \nReason: 11 always doubles \n";
            
        }
        else if (h_in.get_val_of_hand() == 10){
            // 10 doubles against dealer 2 through 9 otherwise hit.
            if (d_in.hand[1].get_rank() == "2" || d_in.hand[1].get_rank() == "3" ||
                d_in.hand[1].get_rank() == "4" || d_in.hand[1].get_rank() == "5" ||
                d_in.hand[1].get_rank() == "6" || d_in.hand[1].get_rank() == "7" ||
                d_in.hand[1].get_rank() == "9" || d_in.hand[1].get_rank() == "9"){
                cout << "Correct choice: Double Down \nReason: 10 doubles against dealer 2 - 9 \n";
            }
            
            else { cout << "Correct choice: Hit \nReason: 10 hits against dealer 10 - A\n"; }
            
            
        }
        else if (h_in.get_val_of_hand() == 9){
            // 9 doubles against dealer 3 through 6 otherwise hit.
            if (d_in.hand[1].get_rank() == "3" || d_in.hand[1].get_rank() == "4" ||
                d_in.hand[1].get_rank() == "5" || d_in.hand[1].get_rank() == "6"){
                cout << "Correct choice: Double Down \nReason: 9 doubles against dealer 3 - 6 \n";
            }
            
            else { cout << "Correct choice: Hit \nReason: 9 hits against dealer A - 2 and dealer 7 - K \n"; }
            
        }
        else if (h_in.get_val_of_hand() <= 8){
            // 8 always hits
            cout << "Correct choice: Hit \nReason: 8 always hits \n";
        }
    }
}


// EFFECTS: Compares the two hands and determines who wins
void compare(const Player &d_in, const Player &h_in){
    
    print_hands(d_in, h_in, false, true); // NOT the first round, IS the last round
    
    cout << endl;
    
    if (d_in.bust()){ cout << "Dealer busted. Human wins!\n"; } // Dealer has greater than 21
    else if (h_in.bust()){ cout << "Human busted. Dealer Wins!\n"; } // Human has greater than 21
    
    // At this point, both players must have a score less than 21
    
    else if (d_in.get_val_of_hand() > h_in.get_val_of_hand()){ cout << "Dealer's hand is better than the Human's hand. Dealer Wins!\n"; }
    else if (d_in.get_val_of_hand() < h_in.get_val_of_hand()){ cout << "Human's hand is better than the Dealer's hand. Human Wins!\n"; }
    
    else { cout << "Push\n"; } // Values are the same
}








