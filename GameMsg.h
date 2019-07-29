#ifndef __GAMESERVER_GAMEMSG_H__
#define __GAMESERVER_GAMEMSG_H__
#include "zinx.h"
#include "GameProtocol.h"
#include "google/protobuf/message.h"
#include "msg.GameMsgType.h"

class GameMsg : public UserData {
 public:
  //��Ա��ϢID
  enum MSG_TYPE {
    MSG_TYPE_LOGIN_ID = 1,         //���id������(����֪ͨ)
    MSG_TYPE_TALK_CONTENT = 2,     //��������
    MSG_TYPE_NEW_POSITION = 3,     //��λ��
    MSG_TYPE_BROAD_CAST = 200,     //�㲥��Ϣ
    MSG_TYPE_LOGOFF_ID = 201,      //���id������(����֪ͨ)
    MSG_TYPE_SRD_PLAYERS =202      //��Χ���λ��
  } m_msg_type;

  //���캯��,���ֽ���(�洢��string��)ת���ɶ���
  GameMsg(MSG_TYPE _msg_type, std::string Value);
  //���캯��,���ڹ���յ���Ϣ
  explicit GameMsg(MSG_TYPE _type);
  //��������
  virtual ~GameMsg();

  //���л�����
  std::string Serielize();
  //��ȡ������Ϣ����
  std::string GetTalkContent();

  //protobuf�е���Ϣ����ָ��(����ģʽ)
  google::protobuf::Message* pMsgContent;

};

#endif
