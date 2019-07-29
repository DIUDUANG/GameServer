#include "AOI_World.h"

AOI_World::AOI_World(
  int _x_begin, 
  int _x_end, 
  int _y_begin, 
  int _y_end, 
  int _x_count, 
  int _y_count) 
    : x_begin(_x_begin),
      x_end(_x_end),
      y_begin(_y_begin),
      y_end(_y_end),
      x_count(_x_count),
      y_count(_y_count) {

  //创建所有空网格
  for (int i = 0; i < y_count * x_count; ++i) {
    AOI_Grid tmp;
    m_grids.push_back(tmp);
  }

}

AOI_World::~AOI_World() {
}

std::list<AOI_Player*> AOI_World::GetSrdPlayers(AOI_Player* _player) {
  std::list<AOI_Player*> _grd_players;
  //将周围玩家放到_grd_players中

  //计算玩家所属网格
  int gid = (_player->get_x() - x_begin) / GetXWidth() 
              + (_player->get_y() - y_begin) / GetYWidth() * x_count;

  //计算该格子属于x轴第几个,并同时计算属于y轴第几个
  int x_index = gid % x_count;
  int y_index = gid / x_count;

  //有左上角的玩家,则将所有玩家放入_grd_player中
  if (x_index > 0 && y_index > 0) {
    auto playerList = m_grids[gid - 1 - x_count].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //上边
  if (y_index > 0) {
    auto playerList = m_grids[gid - x_count].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //右上角
  if (x_index < x_count - 1 && y_index > 0) {
    auto playerList = m_grids[gid - x_count + 1].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //左边
  if (x_index > 0) {
    auto playerList = m_grids[gid - 1].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //自己
  auto playerList = m_grids[gid].GetList();
  _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());

  //右边
  if (x_index < x_count-1) {
    auto playerList = m_grids[gid + 1].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //左下角
  if (x_index > 0 && y_index < y_count - 1) {
    auto playerList = m_grids[gid - 1 + x_count].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //下边
  if (y_index < y_count - 1) {
    auto playerList = m_grids[gid + x_count].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //右下角
  if (x_index < x_count - 1 && y_index < y_count - 1) {
    auto playerList = m_grids[gid + 1 + x_count].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }
  

  return _grd_players;
}

bool AOI_World::AddPlayer(AOI_Player* _player) {
  //计算玩家所属网格
  int gid = (_player->get_x() - x_begin) / GetXWidth()
    + (_player->get_y() - y_begin) / GetYWidth() * x_count;

  m_grids[gid].AddPlayer(_player);
  return true;
}

bool AOI_World::DelPlayer(AOI_Player* _player) {
  //计算玩家所属网格
  int gid = (_player->get_x() - x_begin) / GetXWidth()
    + (_player->get_y() - y_begin) / GetYWidth() * x_count;
  m_grids[gid].DelPlayer(_player);

  return true;
}

int AOI_World::GetXWidth() {

  return (x_end - x_begin) / x_count;
}

int AOI_World::GetYWidth() {
  return (y_end - y_begin) / y_count;
}
