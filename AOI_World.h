#ifndef __GAMESERVER_AOI_WORLD_H__yyp
#define __GAMESERVER_AOI_WORLD_H__yyp
#define DISALLOW_COPY_AND_ASSIGN(Typename) \
        Typename(const Typename&);  \
        void operator=(const Typename&)
#include <vector>
#include "AOI_Grid.h"

class AOI_World {
 public:
   //构造
   AOI_World(int _x_begin,
     int _x_end,
     int _y_begin,
     int _y_end,
     int _x_count,
     int _y_count);
   //析构
   ~AOI_World();

   //获取某玩家周围玩家
   std::list<AOI_Player*> GetSrdPlayers(AOI_Player* _player);
   //添加玩家到游戏世界
   bool AddPlayer(AOI_Player* _player);
   //摘除玩家
   bool DelPlayer(AOI_Player* _player);

 private:
   //获取x轴网格宽度
   int GetXWidth();
   //获取y轴网格宽度
   int GetYWidth();

  //存放若干格子对象
  std::vector<AOI_Grid> m_grids;

  //矩形地图的参数
  int x_begin;  //x轴起始坐标
  int x_end;    //x轴结束坐标
  int y_begin;  //y轴起始坐标
  int y_end;    //y轴结束坐标

  //地图的格子数
  int x_count;  //x轴的格子数
  int y_count;  //y轴的格子数

  //防止生成拷贝构造和重载赋值运算符
  DISALLOW_COPY_AND_ASSIGN(AOI_World);
};

#endif