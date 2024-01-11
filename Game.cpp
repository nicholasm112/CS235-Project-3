/*
Nicholas Morgan
Project 3
*/
#include "Game.h"
#include <iostream>
#include <string>


Game::Game(PlayerComponents player0, PlayerComponents player1,
		PlayerComponents player2, PlayerComponents player3, Pack pack) {

        }

void run() {

}
void deal();
void bid();
void play();
bool score();

inline int next_player(int player) const;
inline int get_team(int player) const;
inline int get_pos_in_team(int player) const;

std::vector<Team> teams;
Pack pack;
int dealer;

static const int LOSING_POINTS = -200;
static const int WINNING_POINTS = 300;

