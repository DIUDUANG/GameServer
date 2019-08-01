#include "GameRole.h"
#include <iostream>
#include <random>
#include <algorithm>
#include "MultiMsg.h"
#include "AOI_World.h"
#include "msg.GameMsgType.h"
#include "NamePool.h"
#include "ExitFrameworkTask.h"
#include "TimeoutDeliver.h"
#include "hiredis/hiredis.h"


namespace {
AOI_World g_world(0, 400, 0, 400, 20, 20);
int g_curID = 0;
std::default_random_engine randSrc(time(NULL));
ExitFrameworkTask* peft = NULL;
}



GameRole::GameRole() 
    : pGameProtocol(NULL), 
      iPid(0),
      x(0),
      y(0),
      z(0),
      v(0){
  x = 200 + randSrc() % 50;
  z = 100 + randSrc() % 10;
  m_name = NamePool::GetInstance().GetName();
  iPid = g_curID++;
}

GameRole::~GameRole() {
  NamePool::GetInstance().ReleaseName(m_name);
}

void GameRole::BindProtocol(Iprotocol* _pProtocol) {
  pGameProtocol = _pProtocol;
}

//新客户端连接后
bool GameRole::Init() {
  //若是第一个玩家-->删除退出定时任务
  if (ZinxKernel::Zinx_GetAllRole().size() <= 0) {
    if (NULL != peft) {
      TimeoutDeliver::GetInstance()->UnregisterTask(peft);
      delete peft;
      peft = NULL;
    }
  }

  //将玩家添加到游戏世界中
  g_world.AddPlayer(this);

  //1.服务器向其发送ID和名称
  GameMsg* pLoginMsg = MakeLoginIDName();
  ZinxKernel::Zinx_SendOut(*pLoginMsg, *pGameProtocol);

  //2.服务器向其发送周围玩家位置
  GameMsg* pSrdPlayers = MakeSrdPlayers();
  ZinxKernel::Zinx_SendOut(*pSrdPlayers, *pGameProtocol);

  //3.向周围玩家发送其位置
  //获取周围玩家
  std::list<AOI_Player*> players_list = g_world.GetSrdPlayers(this);
  //循环发送
  for (auto r : players_list) {
    //构造自己出生位置信息
    GameMsg* pInitPos = MakeInitPos();
    ZinxKernel::Zinx_SendOut(
      *pInitPos,
      *(dynamic_cast<GameRole*>(r)->pGameProtocol));
  }

  //添加昵称到redis数据库 GameName链表中
  auto context = redisConnect("127.0.0.1", 6379);
  if (NULL != context) {
    //向redis数据插入玩家姓名
    auto cmd = redisCommand(context, "lpush GameName %s", this->m_name.c_str());
    //释放命令对象
    freeReplyObject(cmd);
    cmd = NULL;
    //释放连接
    redisFree(context);
  }

  return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData) {
  //参数来自于GameProtocol->raw2request的返回值(MutliMessage)
  GET_REF2DATA(MultiMsg, data, _poUserData);
  std::string talkContent;
  for (auto r : data._multi_msg) {
    //根据消息类型不同进行相应的处理
    switch (r->m_msg_type) {
      case GameMsg::MSG_TYPE_TALK_CONTENT:
        //构造广播聊天消息-->发给所有玩家
        talkContent = r->GetTalkContent();
        for (auto role : ZinxKernel::Zinx_GetAllRole()) {
          GameRole* player = dynamic_cast<GameRole*>(role);
          GameMsg* poutMsg = MakeTalkBroadCast(talkContent);
          ZinxKernel::Zinx_SendOut(*poutMsg, *(player->pGameProtocol));
        } 
        break;

      case GameMsg::MSG_TYPE_NEW_POSITION:
        /*处理移动处理*/
        //取出坐标信息
        ProcNewPosition(
          dynamic_cast<GameMsgType::Position*>(r->pMsgContent)->x(),
          dynamic_cast<GameMsgType::Position*>(r->pMsgContent)->y(),
          dynamic_cast<GameMsgType::Position*>(r->pMsgContent)->z(),
          dynamic_cast<GameMsgType::Position*>(r->pMsgContent)->v());
        break;

      default:
        break;
    }
  }
 

  return NULL;
}

void GameRole::Fini() {
  //客户端断开时,向周围玩家发送其断开的消息
  //获取周围玩家,循环发送
  std::list<AOI_Player*> player_list = g_world.GetSrdPlayers(this);
  for (auto r : player_list) {
    GameRole* player = dynamic_cast<GameRole*>(r);
    GameMsg* logOffMsg = MakeLogoff();
    ZinxKernel::Zinx_SendOut(*logOffMsg, *(player->pGameProtocol));
  }

  //从游戏世界中删除玩家
  g_world.DelPlayer(this);

  //判断如果是最后一个人--->调起退出定时器  
  if (ZinxKernel::Zinx_GetAllRole().size() <= 1) {
    peft = new ExitFrameworkTask();
    TimeoutDeliver::GetInstance()->RegisterTask(20, peft);
  }

  //从redis数据库中删除玩家姓名
  auto context = redisConnect("127.0.0.1  ", 6379);
  if (NULL != context) {
    //从redis数据库删除玩家姓名
    auto cmd = redisCommand(context, "lrem GameName 1 %s", this->m_name.c_str());
    //释放命令对象
    freeReplyObject(cmd);
    cmd = NULL;
    //释放连接
    redisFree(context);
  }


}

GameMsg* GameRole::MakeLoginIDName() {
  GameMsg* ret = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID);
  //将GameRole属性赋值给ret->pMsgContent
  GameMsgType::SyncPid* syncpidMsg = dynamic_cast<GameMsgType::SyncPid*>(ret->pMsgContent);
  syncpidMsg->set_pid(iPid);
  syncpidMsg->set_username(m_name);

  return ret;
}

GameMsg* GameRole::MakeSrdPlayers() {
  GameMsg* ret = new GameMsg(GameMsg::MSG_TYPE::MSG_TYPE_SRD_PLAYERS);
  //将GameRole属性赋值给ret->pMsgContent
  auto syncPlayerMsg = dynamic_cast<GameMsgType::SyncPlayers*>(ret->pMsgContent);
  //获取周围玩家,循环设置到syncPlayerMsg
  std::list<AOI_Player*> player_list = g_world.GetSrdPlayers(this);
  for (auto r : player_list) {

    GameMsgType::Player* single_player = syncPlayerMsg->add_ps();
    GameRole* player = dynamic_cast<GameRole*>(r);
    single_player->set_pid(player->iPid);
    single_player->set_username(player->m_name);
    auto position = single_player->mutable_p();
    position->set_x(player->x);
    position->set_y(player->y);
    position->set_z(player->z);
    position->set_v(player->v);
  }

  return ret;
}

GameMsg* GameRole::MakeInitPos() {
  GameMsg* ret = new GameMsg(GameMsg::MSG_TYPE_BROAD_CAST);

  //设置广播消息类型字段为2,设置自己的坐标到消息里
  GameMsgType::BroadCast* iniPosMsg = dynamic_cast<GameMsgType::BroadCast*>(ret->pMsgContent);
  iniPosMsg->set_pid(iPid);
  iniPosMsg->set_username(m_name);
  iniPosMsg->set_tp(2);

  auto position = iniPosMsg->mutable_p();
  position->set_x(x);
  position->set_y(y);
  position->set_z(z);
  position->set_v(v);

  return ret;
}

GameMsg* GameRole::MakeLogoff() {
  GameMsg* ret = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID);
  GameMsgType::SyncPid* logOffMsg = dynamic_cast<GameMsgType::SyncPid*>(ret->pMsgContent);

  logOffMsg->set_pid(iPid);
  logOffMsg->set_username(m_name);

  return ret;
}

GameMsg* GameRole::MakeTalkBroadCast(std::string _talkContent) {
  GameMsg* ret = new GameMsg(GameMsg::MSG_TYPE_BROAD_CAST);
  //设置广播类型是1,吧聊天内容设置到ret里
  auto broadCastTalk = dynamic_cast<GameMsgType::BroadCast*>(ret->pMsgContent);
  broadCastTalk->set_pid(iPid);
  broadCastTalk->set_username(m_name);
  broadCastTalk->set_tp(1);
  broadCastTalk->set_content(_talkContent);

  return ret;
}

GameMsg* GameRole::MakeNewPositionBroadCast() {
  GameMsg* ret = new GameMsg(GameMsg::MSG_TYPE_BROAD_CAST);
  auto newPosMsg = dynamic_cast<GameMsgType::BroadCast*>(ret->pMsgContent);
  newPosMsg->set_pid(iPid);
  newPosMsg->set_tp(4);
  newPosMsg->set_username(m_name);

  GameMsgType::Position* position = newPosMsg->mutable_p();
  position->set_x(x);
  position->set_y(y);
  position->set_z(z);
  position->set_v(v);

  return ret;
}
/*处理移动处理*/
void GameRole::ProcNewPosition(float _x, float _y, float _z, float _v) {

  /*1.跨网格视野处理*/
  //获取旧邻居
  std::list<AOI_Player *> oldSrd = g_world.GetSrdPlayers(this);

  //摘除玩家--->更新坐标--->添加玩家
  g_world.DelPlayer(this);
  x = _x;
  y = _y;
  z = _z;
  v = _v;
  g_world.AddPlayer(this);

  //获取新数据
  std::list<AOI_Player*> newSrd = g_world.GetSrdPlayers(this);

  //遍历旧邻居,在新邻居中查找,没找到-->视野消失
  for (auto single : oldSrd) {
    //find算法会返回查找到的元素的迭代器,没找到旧返回.end()
    if (newSrd.end() == find(newSrd.begin(), newSrd.end(), single)) {
      GameRole* player = dynamic_cast<GameRole*>(single);
      //视野消失
      ViewLost(player);
    }
  }

  //遍历新邻居,在旧邻居中查找,没找到--->视野出现
  for (auto single : newSrd) {
    if (oldSrd.end() == find(oldSrd.begin(), oldSrd.end(), single)) {
      GameRole* player = dynamic_cast<GameRole*>(single);
      //视野出现
      ViewAppear(player);
    }
  }

  /*2.广播新位置给周围玩家*/
  for (auto single : newSrd) {
    auto pMsg = MakeNewPositionBroadCast();
    auto player = dynamic_cast<GameRole*>(single);
    ZinxKernel::Zinx_SendOut(*pMsg, *(player->pGameProtocol));
  }
  
}

void GameRole::ViewLost(GameRole* _oldSrd) {
  //向旧邻居循环法自己的下线消息
    GameMsg* logOffMsg = this->MakeLogoff();
    ZinxKernel::Zinx_SendOut(*logOffMsg, *(_oldSrd->pGameProtocol));

  //向自己发旧邻居的下线消息

    logOffMsg = _oldSrd->MakeLogoff();
    ZinxKernel::Zinx_SendOut(*logOffMsg, *(this->pGameProtocol));

}

void GameRole::ViewAppear(GameRole* _newSrd) {
  //循环发送自己的出现消息(广播出生位置)给新邻居

    GameMsg* appear = this->MakeInitPos();
    ZinxKernel::Zinx_SendOut(*appear, *(_newSrd->pGameProtocol));

  //循环发送邻居的出现消息给自己

    appear = _newSrd->MakeInitPos();
    ZinxKernel::Zinx_SendOut(*appear, *(this->pGameProtocol));

}

int GameRole::get_x() {
  return static_cast<int>(x);
}

int GameRole::get_y() {
  return static_cast<int>(z);
}
