/*
Nicholas Morgan
Project 3
*/
#include "Team.h"
#include <iostream>
#include <string>



Team::Team(const PlayerComponents& player1, const PlayerComponents& player2){
    total_bags = 0;
    game_score = 0;
    team.push_back(Player_factory(player1.name, player1.strategy));
    team.push_back(Player_factory(player2.name, player2.strategy));
}

// Wrapper functions on Player class 
// ---------------------------------------------------------------------------
/* Call get_name on a player in the team;
* @pre valid player value is either 0 or 1
* @param player which player to call the function on
*/
std::string Team::get_name(int player) const {
    if (player == 0 or player == 1) {
       return team[player]->get_name();
    }
    else {
        throw std::out_of_range("Not a valid player.");
        
    }
    
}

/* Call set_bid on a player in the team;
* @pre valid player value is either 0 or 1
* @param player which player to call the function on
*/
void Team::set_bid(int player) {
    if (player == 0 or player == 1) {
        return team[player]->set_bid();
    }
    else {
        throw std::out_of_range("Not a valid player.");

    }
}

/* Call add_trick on a player in the team
* @pre valid player value is either 0 or 1
* @param player which player to call the function on
*/
bool Team::add_trick(int player) {
    if (player == 0 or player == 1) {
        return team[player]->add_trick(); 
        
    }
    else {
        throw std::out_of_range("Not a valid player.");
        
        
    }
    
}

/* Call add_card on a player in the team
* @pre valid player value is either 0 or 1
* @param player which player to call the function on
* @param card Card to add to the hand of player
*/
bool Team::add_card(int player, const Card& card) {//
    if (player == 0 or player == 1) {
        return team[player]->add_card(card);
    }
    else {
        throw std::out_of_range("Not a valid player.");
        
    }

}

Card Team::lead_card(int player, bool broken_spade) {
    if (player == 0 or player == 1) {
        return team[player]->lead_card(broken_spade);
    }
    else {
        throw std::out_of_range("Not a valid player.");
    }
}

Card Team::play_card(int player, Suit led_suit) {
    if (player == 0 or player == 1) {
        return team[player]->play_card(led_suit);
    }
    else {
        throw std::out_of_range("Not a valid player.");
    }
}

/* Call reset on all the players of the team
*/
void Team::reset() {
    team[0]->reset();
    team[1]->reset();
}

// Team functions
// ---------------------------------------------------------------------------
// Returns the score of the team
Score Team::get_score() {
    Score score;
    int possible_bag = 0;
    int n_nil_tricks = 0;
    score.nil_score = calculate_nil_score(score.tricks,n_nil_tricks,score.bid,possible_bag);
    score.bid_score = calculate_bid_score(score.tricks,score.bid,score.bags,possible_bag);
    score.total_bags = update_total_bags(score.bags,score.bag_penalty);
    score.hand_score = calculate_hand_score(score.bid_score,score.nil_score,score.bag_penalty);
    score.game_score = update_game_score(score.hand_score);
    return score;
}

Team::~Team() {
    team.clear();
}

std::vector<Player*> team;
int total_bags;
int game_score;

// Returns true if the player value is 0 or 1
inline bool Team::valid_player(int player) const{
    if (player == 0 or player == 1) {
        return true;
    }
    return false;
}

// Calculate the nil score of the team
// Modifies the tricks, bid, and possible bags 
/*
For every player in the team, regardless of what they bid, add the total tricks and bid to the `tricks` and `bid` parameters.
- If the player bid nil, then calculate their nil_score based on the type of nil they bid and add it to the `nil_score` parameter
	- The absolute value of a blind nil score is 200.
	- The absolute value of a regular nil score is 100.
	- The calculation of the player's nil_score is the same regardless of the type of nil. The only difference is the absolute values based on the type of nil bid they have.
	- If the player who bid any type of nil, didn't recieve any tricks then their nil score is positive
	- Otherwise, their nil score is negative and the number of tricks they recieved is added to the `possible_bags` parameter.
- If the player didn't bid nil, then add their number of tricks to the `non_nil_tricks` parameter
*/
inline int Team::calculate_nil_score(int& tricks, int& non_nil_tricks, int& bid, int& possible_bags) {
    int nil = 0;
    for (int i = 0; i < team.size(); i++) {
        tricks += team[i]->get_trick();
        bid += team[i]->get_bid();
        if (team[i]->blind_nil_bid()) {
            if (team[i]->get_trick()>0) {
                nil -= 200;
                possible_bags += team[i]->get_trick();
            }
            else {
                nil += 200;
            }
        }
        if (team[i]->nil_bid()) {
            if (team[i]->get_trick()>0) {
                nil -= 100;
                possible_bags += team[i]->get_trick();
            }
            else {
                nil += 100;
            }
        }
        else {
            non_nil_tricks = team[i]->get_trick();

        }
        }
        return nil;
            

        }


// Calculates and return the bid score of the team
// Modifies the bag to correct value
/*
 If both players bid any type of nil, then just return 0
- If the number of `non_nil_tricks` is less than the `bid`,
 then there are no `bags` and `bid_score` is the negation of ten times `bid`
- Else, the number of `bags` is the addition of the `possible_bags`, 
`non_nil_tricks`, and negation of `bid`; the `bid_score` is `bid`
 times ten plus `bags`
*/
inline int Team::calculate_bid_score(int non_nil_tricks, int bid, int& bags, int possible_bags) {
    int bid_score = 0;
    if (team[0]->get_bid() == 0 and team[1]->get_bid() == 0) {
        return 0;
    }
    if(non_nil_tricks < bid) {
        bags = 0;
        bid_score = -(bid*10);
    }
    else {
        bags = ((possible_bags + non_nil_tricks) - bid);
        bid_score = ((bid*10) + bags);
    }
    return bid_score;
}

// Updates the total bags of the team
// Modifies the bag_penalty to correct value
// Returns the total bags
inline int Team::update_total_bags(int bags, int& bag_penalty) {
    total_bags = total_bags + bags;
    if (total_bags >= 10) {
        bag_penalty = -100;
        int i = total_bags%10;
        total_bags = i;
        return total_bags;
    }
    else {
        bag_penalty = 0;
        return total_bags;
    }

}

// Calculates and returns the hand score of the team
inline int Team::calculate_hand_score(int bid_score, int nil_score, int bag_penalty) {
    return  (bid_score + nil_score + bag_penalty);

}

// Updates the game score of the team
// Returns the updated game score
inline int Team::update_game_score(int hand_score) {
    
    return (game_score + hand_score);

}

std::ostream& operator<<(std::ostream& os, const Team& team) {
    
    os << "Team " + team.get_name(0) + "/" + team.get_name(1);
    return os;

}


std::ostream& operator<<(std::ostream& os, const Score& score) {
    std::string str;
    std::string begin[] = {"Bid: ", "Tricks: ", "Bags: ", "Bid Score: ", "Nil Score: ", "Bag Penalty: ", "Hand Score: ", "Total Bags: ", "Game Score: "};
    int sco[] = {score.bid, score.tricks, score.bags, score.bid_score, score.nil_score, score.bag_penalty, score.hand_score, score.total_bags, score.game_score};
    for(int i = 0; i < 10; i++) {
        str = begin[i] + std::to_string(sco[i])+"/n";
        os << str;
    }
    return os; 
    
}



