#include "GameMsg.h"

GameMsg::GameMsg(MSG_TYPE _msg_type, std::string Value) 
  : m_msg_type(_msg_type), pMsgContent(NULL) {

  //根据m_msg_type构造对应的对象
  switch (_msg_type) {

  case MSG_TYPE_LOGIN_ID:     //玩家id和姓名(上线通知)
    pMsgContent = new GameMsgType::SyncPid();
    break;

  case MSG_TYPE_TALK_CONTENT: //聊天内容
    pMsgContent = new GameMsgType::Talk();
    break;

  case MSG_TYPE_NEW_POSITION: //新位置
    pMsgContent = new GameMsgType::Position();
    break;

  case MSG_TYPE_BROAD_CAST:   //广播消息
    pMsgContent = new GameMsgType::BroadCast();
    break;

  case MSG_TYPE_LOGOFF_ID:    //玩家id和姓名(下线通知)
    pMsgContent = new GameMsgType::SyncPid();
    break;

  case MSG_TYPE_SRD_PLAYERS:  //周围玩家位置
    pMsgContent = new GameMsgType::SyncPlayers();
    break;
    
  default:
    break;
  }

  //将序列化数据结构化
  pMsgContent->ParseFromString(Value);
}

GameMsg::GameMsg(MSG_TYPE _type) : m_msg_type(_type) {
  //根据m_msg_type构造对应的对象
  switch (_type) {

    case MSG_TYPE_LOGIN_ID:     //玩家id和姓名(上线通知)
      pMsgContent = new GameMsgType::SyncPid();
      break;

    case MSG_TYPE_TALK_CONTENT: //聊天内容
      pMsgContent = new GameMsgType::Talk();
      break;

    case MSG_TYPE_NEW_POSITION: //新位置
      pMsgContent = new GameMsgType::Position();
      break;

    case MSG_TYPE_BROAD_CAST:   //广播消息
      pMsgContent = new GameMsgType::BroadCast();
      break;

    case MSG_TYPE_LOGOFF_ID:    //玩家id和姓名(下线通知)
      pMsgContent = new GameMsgType::SyncPid();
      break;

    case MSG_TYPE_SRD_PLAYERS:  //周围玩家位置
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
  //将protobuf序列化位字节流
  pMsgContent->SerializeToString(&tmp_string);

  return tmp_string;
}

std::string GameMsg::GetTalkContent() {
  //让成员pMsgContent获取聊天消息内容
  //动态类型转 pMsgContent--->pb::Talk
  GameMsgType::Talk* pTalk = dynamic_cast<GameMsgType::Talk*>(pMsgContent);
  if (NULL == pTalk) {
    return "error:2333333";
  }
  return pTalk->content();
}
