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

  //�������п�����
  for (int i = 0; i < y_count * x_count; ++i) {
    AOI_Grid tmp;
    m_grids.push_back(tmp);
  }

}

AOI_World::~AOI_World() {
}

std::list<AOI_Player*> AOI_World::GetSrdPlayers(AOI_Player* _player) {
  std::list<AOI_Player*> _grd_players;
  //����Χ��ҷŵ�_grd_players��

  //���������������
  int gid = (_player->get_x() - x_begin) / GetXWidth() 
              + (_player->get_y() - y_begin) / GetYWidth() * x_count;

  //����ø�������x��ڼ���,��ͬʱ��������y��ڼ���
  int x_index = gid % x_count;
  int y_index = gid / x_count;

  //�����Ͻǵ����,��������ҷ���_grd_player��
  if (x_index > 0 && y_index > 0) {
    auto playerList = m_grids[gid - 1 - x_count].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //�ϱ�
  if (y_index > 0) {
    auto playerList = m_grids[gid - x_count].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //���Ͻ�
  if (x_index < x_count - 1 && y_index > 0) {
    auto playerList = m_grids[gid - x_count + 1].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //���
  if (x_index > 0) {
    auto playerList = m_grids[gid - 1].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //�Լ�
  auto playerList = m_grids[gid].GetList();
  _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());

  //�ұ�
  if (x_index < x_count-1) {
    auto playerList = m_grids[gid + 1].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //���½�
  if (x_index > 0 && y_index < y_count - 1) {
    auto playerList = m_grids[gid - 1 + x_count].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //�±�
  if (y_index < y_count - 1) {
    auto playerList = m_grids[gid + x_count].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }

  //���½�
  if (x_index < x_count - 1 && y_index < y_count - 1) {
    auto playerList = m_grids[gid + 1 + x_count].GetList();
    _grd_players.insert(_grd_players.end(), playerList.begin(), playerList.end());
  }
  

  return _grd_players;
}

bool AOI_World::AddPlayer(AOI_Player* _player) {
  //���������������
  int gid = (_player->get_x() - x_begin) / GetXWidth()
    + (_player->get_y() - y_begin) / GetYWidth() * x_count;

  m_grids[gid].AddPlayer(_player);
  return true;
}

bool AOI_World::DelPlayer(AOI_Player* _player) {
  //���������������
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
