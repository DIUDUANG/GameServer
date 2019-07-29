#include "GamePlayer.h"

GamePlayer::GamePlayer(int _x, int _y, std::string _name) 
    : x(_x), y(_y), name(_name) {}

int GamePlayer::get_x() {
  return x;
}

int GamePlayer::get_y() {
  return y;
}


