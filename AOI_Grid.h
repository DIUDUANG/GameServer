#ifndef __GAMESERVER_AOI_GRID_H__
#define __GAMESERVER_AOI_GRID_H__
#include <list>
#include "AOI_Player.h"

class AOI_Grid {
 public:
  //构造
   AOI_Grid();
  //析构
   ~AOI_Grid();

  //向网格添加玩家
  void AddPlayer(AOI_Player* _player);
  //从网格中删除玩家
  void DelPlayer(AOI_Player* _player);
  //获取存放玩家
  std::list<AOI_Player*>& GetList();

 private:
  //存放若干玩家对象(有横坐标和纵坐标的对象)
  std::list<AOI_Player*> m_players;

};

#endif