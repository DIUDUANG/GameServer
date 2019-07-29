#ifndef __GAMESERVER_GAMEROLE_H__
#define __GAMESERVER_GAMEROLE_H__
#include "zinx.h"
#include "GameMsg.h"
#include "AOI_Player.h"

class GameRole : public Irole, public AOI_Player {
 public:
  GameRole();
  void BindProtocol(Iprotocol* _pProtocol);

  // ͨ�� AOI_Player �̳�
  virtual int get_x() override;
  virtual int get_y() override;

 private:
  // ͨ�� Irole �̳�
  virtual bool Init() override;
  virtual UserData* ProcMsg(UserData& _poUserData) override;
  virtual void Fini() override;
  
  //����һ������id�����Ƶ���Ϣ
  GameMsg* MakeLoginIDName();
  //������Χ�����Ϣ
  GameMsg* MakeSrdPlayers();
  //����Χ��ҷ�����λ��
  GameMsg* MakeInitPos();
  //�ͻ��˶Ͽ�ʱ,����Χ��ҷ�����Ͽ�����Ϣ
  GameMsg* MakeLogoff();
  //����㲥������Ϣ
  GameMsg* MakeTalkBroadCast(std::string _talkContent);

  Iprotocol* pGameProtocol;
  int iPid;           //���ID
  std::string m_name; //�������
  /*��������*/
  float x;            //������
  float y;            //�߶�
  float z;            //������
  float v;            //�Ƕ�
};


#endif

