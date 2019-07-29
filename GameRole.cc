#include "GameRole.h"
#include "MultiMsg.h"
#include "AOI_World.h"
#include "msg.GameMsgType.h"


static AOI_World g_world(0, 400, 0, 400, 20, 20);
static int g_curID = 0;


GameRole::GameRole() 
    : pGameProtocol(NULL), 
      iPid(0),
      x(0),
      y(0),
      z(0),
      v(0){
  x = 100;
  z = 100;
  m_name = "ccc";
  iPid = g_curID++;
}

void GameRole::BindProtocol(Iprotocol* _pProtocol) {
  pGameProtocol = _pProtocol;
}

//�¿ͻ������Ӻ�
bool GameRole::Init() {
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
        break;

      default:
        break;
    }
  }
 

  return nullptr;
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
  GameMsgType::SyncPlayers* syncPlayerMsg = dynamic_cast<GameMsgType::SyncPlayers*>(ret->pMsgContent);
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

int GameRole::get_x() {
  return static_cast<int>(x);
}

int GameRole::get_y() {
  return static_cast<int>(z);
}
