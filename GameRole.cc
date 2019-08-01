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

//�¿ͻ������Ӻ�
bool GameRole::Init() {
  //���ǵ�һ�����-->ɾ���˳���ʱ����
  if (ZinxKernel::Zinx_GetAllRole().size() <= 0) {
    if (NULL != peft) {
      TimeoutDeliver::GetInstance()->UnregisterTask(peft);
      delete peft;
      peft = NULL;
    }
  }

  //�������ӵ���Ϸ������
  g_world.AddPlayer(this);

  //1.���������䷢��ID������
  GameMsg* pLoginMsg = MakeLoginIDName();
  ZinxKernel::Zinx_SendOut(*pLoginMsg, *pGameProtocol);

  //2.���������䷢����Χ���λ��
  GameMsg* pSrdPlayers = MakeSrdPlayers();
  ZinxKernel::Zinx_SendOut(*pSrdPlayers, *pGameProtocol);

  //3.����Χ��ҷ�����λ��
  //��ȡ��Χ���
  std::list<AOI_Player*> players_list = g_world.GetSrdPlayers(this);
  //ѭ������
  for (auto r : players_list) {
    //�����Լ�����λ����Ϣ
    GameMsg* pInitPos = MakeInitPos();
    ZinxKernel::Zinx_SendOut(
      *pInitPos,
      *(dynamic_cast<GameRole*>(r)->pGameProtocol));
  }

  //����ǳƵ�redis���ݿ� GameName������
  auto context = redisConnect("127.0.0.1", 6379);
  if (NULL != context) {
    //��redis���ݲ����������
    auto cmd = redisCommand(context, "lpush GameName %s", this->m_name.c_str());
    //�ͷ��������
    freeReplyObject(cmd);
    cmd = NULL;
    //�ͷ�����
    redisFree(context);
  }

  return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData) {
  //����������GameProtocol->raw2request�ķ���ֵ(MutliMessage)
  GET_REF2DATA(MultiMsg, data, _poUserData);
  std::string talkContent;
  for (auto r : data._multi_msg) {
    //������Ϣ���Ͳ�ͬ������Ӧ�Ĵ���
    switch (r->m_msg_type) {
      case GameMsg::MSG_TYPE_TALK_CONTENT:
        //����㲥������Ϣ-->�����������
        talkContent = r->GetTalkContent();
        for (auto role : ZinxKernel::Zinx_GetAllRole()) {
          GameRole* player = dynamic_cast<GameRole*>(role);
          GameMsg* poutMsg = MakeTalkBroadCast(talkContent);
          ZinxKernel::Zinx_SendOut(*poutMsg, *(player->pGameProtocol));
        } 
        break;

      case GameMsg::MSG_TYPE_NEW_POSITION:
        /*�����ƶ�����*/
        //ȡ��������Ϣ
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
  //�ͻ��˶Ͽ�ʱ,����Χ��ҷ�����Ͽ�����Ϣ
  //��ȡ��Χ���,ѭ������
  std::list<AOI_Player*> player_list = g_world.GetSrdPlayers(this);
  for (auto r : player_list) {
    GameRole* player = dynamic_cast<GameRole*>(r);
    GameMsg* logOffMsg = MakeLogoff();
    ZinxKernel::Zinx_SendOut(*logOffMsg, *(player->pGameProtocol));
  }

  //����Ϸ������ɾ�����
  g_world.DelPlayer(this);

  //�ж���������һ����--->�����˳���ʱ��  
  if (ZinxKernel::Zinx_GetAllRole().size() <= 1) {
    peft = new ExitFrameworkTask();
    TimeoutDeliver::GetInstance()->RegisterTask(20, peft);
  }

  //��redis���ݿ���ɾ���������
  auto context = redisConnect("127.0.0.1  ", 6379);
  if (NULL != context) {
    //��redis���ݿ�ɾ���������
    auto cmd = redisCommand(context, "lrem GameName 1 %s", this->m_name.c_str());
    //�ͷ��������
    freeReplyObject(cmd);
    cmd = NULL;
    //�ͷ�����
    redisFree(context);
  }


}

GameMsg* GameRole::MakeLoginIDName() {
  GameMsg* ret = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID);
  //��GameRole���Ը�ֵ��ret->pMsgContent
  GameMsgType::SyncPid* syncpidMsg = dynamic_cast<GameMsgType::SyncPid*>(ret->pMsgContent);
  syncpidMsg->set_pid(iPid);
  syncpidMsg->set_username(m_name);

  return ret;
}

GameMsg* GameRole::MakeSrdPlayers() {
  GameMsg* ret = new GameMsg(GameMsg::MSG_TYPE::MSG_TYPE_SRD_PLAYERS);
  //��GameRole���Ը�ֵ��ret->pMsgContent
  auto syncPlayerMsg = dynamic_cast<GameMsgType::SyncPlayers*>(ret->pMsgContent);
  //��ȡ��Χ���,ѭ�����õ�syncPlayerMsg
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

  //���ù㲥��Ϣ�����ֶ�Ϊ2,�����Լ������굽��Ϣ��
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
  //���ù㲥������1,�������������õ�ret��
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
/*�����ƶ�����*/
void GameRole::ProcNewPosition(float _x, float _y, float _z, float _v) {

  /*1.��������Ұ����*/
  //��ȡ���ھ�
  std::list<AOI_Player *> oldSrd = g_world.GetSrdPlayers(this);

  //ժ�����--->��������--->������
  g_world.DelPlayer(this);
  x = _x;
  y = _y;
  z = _z;
  v = _v;
  g_world.AddPlayer(this);

  //��ȡ������
  std::list<AOI_Player*> newSrd = g_world.GetSrdPlayers(this);

  //�������ھ�,�����ھ��в���,û�ҵ�-->��Ұ��ʧ
  for (auto single : oldSrd) {
    //find�㷨�᷵�ز��ҵ���Ԫ�صĵ�����,û�ҵ��ɷ���.end()
    if (newSrd.end() == find(newSrd.begin(), newSrd.end(), single)) {
      GameRole* player = dynamic_cast<GameRole*>(single);
      //��Ұ��ʧ
      ViewLost(player);
    }
  }

  //�������ھ�,�ھ��ھ��в���,û�ҵ�--->��Ұ����
  for (auto single : newSrd) {
    if (oldSrd.end() == find(oldSrd.begin(), oldSrd.end(), single)) {
      GameRole* player = dynamic_cast<GameRole*>(single);
      //��Ұ����
      ViewAppear(player);
    }
  }

  /*2.�㲥��λ�ø���Χ���*/
  for (auto single : newSrd) {
    auto pMsg = MakeNewPositionBroadCast();
    auto player = dynamic_cast<GameRole*>(single);
    ZinxKernel::Zinx_SendOut(*pMsg, *(player->pGameProtocol));
  }
  
}

void GameRole::ViewLost(GameRole* _oldSrd) {
  //����ھ�ѭ�����Լ���������Ϣ
    GameMsg* logOffMsg = this->MakeLogoff();
    ZinxKernel::Zinx_SendOut(*logOffMsg, *(_oldSrd->pGameProtocol));

  //���Լ������ھӵ�������Ϣ

    logOffMsg = _oldSrd->MakeLogoff();
    ZinxKernel::Zinx_SendOut(*logOffMsg, *(this->pGameProtocol));

}

void GameRole::ViewAppear(GameRole* _newSrd) {
  //ѭ�������Լ��ĳ�����Ϣ(�㲥����λ��)�����ھ�

    GameMsg* appear = this->MakeInitPos();
    ZinxKernel::Zinx_SendOut(*appear, *(_newSrd->pGameProtocol));

  //ѭ�������ھӵĳ�����Ϣ���Լ�

    appear = _newSrd->MakeInitPos();
    ZinxKernel::Zinx_SendOut(*appear, *(this->pGameProtocol));

}

int GameRole::get_x() {
  return static_cast<int>(x);
}

int GameRole::get_y() {
  return static_cast<int>(z);
}
