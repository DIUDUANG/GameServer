#include "GameProtocol.h"
#include "GameMsg.h"
#include "MultiMsg.h"

GameProtocol::GameProtocol() : pGameChannel(NULL), pGameRole(NULL){
}

GameProtocol::~GameProtocol() {
  //连接断开--->析构协议对象---->摘除role对象--->释放
  ZinxKernel::Zinx_Del_Role(*pGameRole);
  delete pGameRole;
  pGameRole = NULL;
}

void GameProtocol::BindChannel(Ichannel* _pChannel) {
  pGameChannel = _pChannel;
}

void GameProtocol::BindRole(Irole* _pRole) {
  pGameRole = _pRole;
}

UserData* GameProtocol::raw2request(std::string _szInput) {
  //原始数据(TLV)转化到--->返回包含若干用户请求的GameMsg对象容器的类
  MultiMsg* pret = new MultiMsg();
  //存在粘包现象
  
  //组合上一次报文
  last_message.append(_szInput);

  while (1) {

    /*判断报文是否够长*/
    //1.小于最小长度
    if (last_message.size() < 8) {
      break;
    }

    //2.大于或等于最大长度
    //取出长度和id
    unsigned int length = (unsigned int)(unsigned char)last_message[0];
    length |= ((unsigned int)(unsigned char)last_message[1]) << 8;
    length |= ((unsigned int)(unsigned char)last_message[2]) << 16;
    length |= ((unsigned int)(unsigned char)last_message[3]) << 24;

    unsigned int id = (unsigned int)(unsigned char)last_message[4];
    id |= ((unsigned int)(unsigned char)last_message[5]) << 8;
    id |= ((unsigned int)(unsigned char)last_message[6]) << 16;
    id |= ((unsigned int)(unsigned char)last_message[7]) << 24;


    /*判断数据部分长度是否够长*/
    //1.长度不够长
    if (length > last_message.size() - 8) {
      break;
    }

    //2.长度够长
    //获取数据部分
    std::string msg_substring = last_message.substr(8, length);
    GameMsg* _msg = new GameMsg((GameMsg::MSG_TYPE)id, msg_substring);

    /*判断后续是否还有报文*/
    pret->_multi_msg.push_back(_msg);
    //弹出已处理报文
    last_message.erase(0, 8 + length);

  }
 
  return pret;
}

std::string* GameProtocol::response2raw(UserData& _oUserData) {
  /*游戏消息(GameMsg)--->字节流*/
  std::string* pret = new std::string();

  GET_REF2DATA(GameMsg, data, _oUserData);

  std::string _msg_byte = data.Serielize();
  //拼接消息长度和消息类型
  unsigned int length = _msg_byte.size();
  unsigned int id = data.m_msg_type;

  //追加长度
  pret->push_back(length & 0xff);
  pret->push_back((length >> 8) & 0xff);
  pret->push_back((length >> 16) & 0xff);
  pret->push_back((length >> 24) & 0xff);

  //追加ID
  pret->push_back(id & 0xff);
  pret->push_back((id >> 8) & 0xff);
  pret->push_back((id >> 16) & 0xff);
  pret->push_back((id >> 24) & 0xff);

  //追加消息内容
  pret->append(_msg_byte);

  return pret;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg) {
  return pGameRole;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes) {
  return pGameChannel;
}
