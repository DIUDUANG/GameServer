#ifndef __GAMESERVER_GAMEPLAYER_H__
#define __GAMESERVER_GAMEPLAYER_H__
#define DISALLOW_COPY_AND_ASSIGN(Typename) \
        Typename(const Typename&);  \
        void operator=(const Typename&)
#include <string>
#include "AOI_Player.h"

//游戏玩家类,继承于AOI_PLAYER
class GamePlayer : public AOI_Player {
 public:
  //构造函数
  GamePlayer(int _x, int _y, std::string _name);
 // 通过 AOI_Player 继承
  //获取x轴坐标
  virtual int get_x() override;
  //获取y轴坐标
  virtual int get_y() override;
  
  std::string name;   //昵称

 private:
  int x;              //横坐标
  int y;              //纵坐标


  //防止生成拷贝构造和重载赋值运算符
  DISALLOW_COPY_AND_ASSIGN(GamePlayer);
};

#endif