#ifndef __GAMESERVER_AOI_GRID_H__
#define __GAMESERVER_AOI_GRID_H__
#include <list>
#include "AOI_Player.h"

class AOI_Grid {
 public:
  //����
   AOI_Grid();
  //����
   ~AOI_Grid();

  //������������
  void AddPlayer(AOI_Player* _player);
  //��������ɾ�����
  void DelPlayer(AOI_Player* _player);
  //��ȡ������
  std::list<AOI_Player*>& GetList();

 private:
  //���������Ҷ���(�к������������Ķ���)
  std::list<AOI_Player*> m_players;

};

#endif