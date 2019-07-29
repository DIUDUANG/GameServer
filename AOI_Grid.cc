#include "AOI_Grid.h"

AOI_Grid::AOI_Grid() {
}

AOI_Grid::~AOI_Grid() {
  for (auto& r : m_players) {
    delete r;
    r = NULL;
  }
}

void AOI_Grid::AddPlayer(AOI_Player* _player) {
  m_players.push_back(_player);
}

void AOI_Grid::DelPlayer(AOI_Player* _player) {
  m_players.remove(_player);
}

std::list<AOI_Player*>& AOI_Grid::GetList() {
  return this->m_players;
}
