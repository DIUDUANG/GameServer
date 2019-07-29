#ifndef __GAMESERVER_GAMEMSG_H__
#define __GAMESERVER_GAMEMSG_H__
#include "zinx.h"
#include "GameProtocol.h"
#include "google/protobuf/message.h"
#include "msg.GameMsgType.h"

class GameMsg : public UserData {
 public:
  //成员消息ID
  enum MSG_TYPE {
    MSG_TYPE_LOGIN_ID = 1,         //玩家id和姓名(上线通知)
    MSG_TYPE_TALK_CONTENT = 2,     //聊天内容
    MSG_TYPE_NEW_POSITION = 3,     //新位置
    MSG_TYPE_BROAD_CAST = 200,     //广播消息
    MSG_TYPE_LOGOFF_ID = 201,      //玩家id和姓名(下线通知)
    MSG_TYPE_SRD_PLAYERS =202      //周围玩家位置
  } m_msg_type;

  //构造函数,将字节流(存储在string中)转换成对象
  GameMsg(MSG_TYPE _msg_type, std::string Value);
  //构造函数,用于构造空的消息
  explicit GameMsg(MSG_TYPE _type);
  //析构函数
  virtual ~GameMsg();

  //序列化函数
  std::string Serielize();
  //获取聊天消息内容
  std::string GetTalkContent();

  //protobuf中的消息类型指针(代理模式)
  google::protobuf::Message* pMsgContent;

};

#endif
