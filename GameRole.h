#ifndef __GAMESERVER_GAMEROLE_H__
#define __GAMESERVER_GAMEROLE_H__
#include "zinx.h"
#include "GameMsg.h"
#include "AOI_Player.h"

class GameRole : public Irole, public AOI_Player {
 public:
  GameRole();
  void BindProtocol(Iprotocol* _pProtocol);

  // 通过 AOI_Player 继承
  virtual int get_x() override;
  virtual int get_y() override;

 private:
  // 通过 Irole 继承
  virtual bool Init() override;
  virtual UserData* ProcMsg(UserData& _poUserData) override;
  virtual void Fini() override;
  
  //构造一个包含id和名称的消息
  GameMsg* MakeLoginIDName();
  //构造周围玩家消息
  GameMsg* MakeSrdPlayers();
  //向周围玩家发送其位置
  GameMsg* MakeInitPos();
  //客户端断开时,向周围玩家发送其断开的消息
  GameMsg* MakeLogoff();
  //构造广播聊天消息
  GameMsg* MakeTalkBroadCast(std::string _talkContent);

  Iprotocol* pGameProtocol;
  int iPid;           //玩家ID
  std::string m_name; //玩家姓名
  /*定义坐标*/
  float x;            //横坐标
  float y;            //高度
  float z;            //纵坐标
  float v;            //角度
};


#endif

