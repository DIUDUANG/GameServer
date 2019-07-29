#ifndef __GAMESERVER_AOI_PLAYER_H__
#define __GAMESERVER_AOI_PLAYER_H__
#include <string>

class AOI_Player {
public:
   virtual int get_x() = 0;   //获取玩家横坐标
   virtual int get_y() = 0;   //获取玩家纵坐标
};

#endif