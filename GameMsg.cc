#include "GameMsg.h"

GameMsg::GameMsg(MSG_TYPE _msg_type, std::string Value) 
  : m_msg_type(_msg_type), pMsgContent(NULL) {

  //����m_msg_type�����Ӧ�Ķ���
  switch (_msg_type) {

  case MSG_TYPE_LOGIN_ID:     //���id������(����֪ͨ)
    pMsgContent = new GameMsgType::SyncPid();
    break;

  case MSG_TYPE_TALK_CONTENT: //��������
    pMsgContent = new GameMsgType::Talk();
    break;

  case MSG_TYPE_NEW_POSITION: //��λ��
    pMsgContent = new GameMsgType::Position();
    break;

  case MSG_TYPE_BROAD_CAST:   //�㲥��Ϣ
    pMsgContent = new GameMsgType::BroadCast();
    break;

  case MSG_TYPE_LOGOFF_ID:    //���id������(����֪ͨ)
    pMsgContent = new GameMsgType::SyncPid();
    break;

  case MSG_TYPE_SRD_PLAYERS:  //��Χ���λ��
    pMsgContent = new GameMsgType::SyncPlayers();
    break;
    
  default:
    break;
  }

  //�����л����ݽṹ��
  pMsgContent->ParseFromString(Value);
}

GameMsg::GameMsg(MSG_TYPE _type) : m_msg_type(_type) {
  //����m_msg_type�����Ӧ�Ķ���
  switch (_type) {

    case MSG_TYPE_LOGIN_ID:     //���id������(����֪ͨ)
      pMsgContent = new GameMsgType::SyncPid();
      break;

    case MSG_TYPE_TALK_CONTENT: //��������
      pMsgContent = new GameMsgType::Talk();
      break;

    case MSG_TYPE_NEW_POSITION: //��λ��
      pMsgContent = new GameMsgType::Position();
      break;

    case MSG_TYPE_BROAD_CAST:   //�㲥��Ϣ
      pMsgContent = new GameMsgType::BroadCast();
      break;

    case MSG_TYPE_LOGOFF_ID:    //���id������(����֪ͨ)
      pMsgContent = new GameMsgType::SyncPid();
      break;

    case MSG_TYPE_SRD_PLAYERS:  //��Χ���λ��
      pMsgContent = new GameMsgType::SyncPlayers();
      break;

    default:
      break;
  }

}

GameMsg::~GameMsg() {
  delete pMsgContent;
}

std::string GameMsg::Serielize() {
  std::string tmp_string; 
  //��protobuf���л�λ�ֽ���
  pMsgContent->SerializeToString(&tmp_string);

  return tmp_string;
}

std::string GameMsg::GetTalkContent() {
  //�ó�ԱpMsgContent��ȡ������Ϣ����
  //��̬����ת pMsgContent--->pb::Talk
  GameMsgType::Talk* pTalk = dynamic_cast<GameMsgType::Talk*>(pMsgContent);
  if (NULL == pTalk) {
    return "error:2333333";
  }
  return pTalk->content();
}
