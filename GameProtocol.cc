#include "GameProtocol.h"
#include "GameMsg.h"
#include "MultiMsg.h"

GameProtocol::GameProtocol() : pGameChannel(NULL), pGameRole(NULL){
}

GameProtocol::~GameProtocol() {
  //���ӶϿ�--->����Э�����---->ժ��role����--->�ͷ�
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
  //ԭʼ����(TLV)ת����--->���ذ��������û������GameMsg������������
  MultiMsg* pret = new MultiMsg();
  //����ճ������
  
  //�����һ�α���
  last_message.append(_szInput);

  while (1) {

    /*�жϱ����Ƿ񹻳�*/
    //1.С����С����
    if (last_message.size() < 8) {
      break;
    }

    //2.���ڻ������󳤶�
    //ȡ�����Ⱥ�id
    unsigned int length = (unsigned int)(unsigned char)last_message[0];
    length |= ((unsigned int)(unsigned char)last_message[1]) << 8;
    length |= ((unsigned int)(unsigned char)last_message[2]) << 16;
    length |= ((unsigned int)(unsigned char)last_message[3]) << 24;

    unsigned int id = (unsigned int)(unsigned char)last_message[4];
    id |= ((unsigned int)(unsigned char)last_message[5]) << 8;
    id |= ((unsigned int)(unsigned char)last_message[6]) << 16;
    id |= ((unsigned int)(unsigned char)last_message[7]) << 24;


    /*�ж����ݲ��ֳ����Ƿ񹻳�*/
    //1.���Ȳ�����
    if (length > last_message.size() - 8) {
      break;
    }

    //2.���ȹ���
    //��ȡ���ݲ���
    std::string msg_substring = last_message.substr(8, length);
    GameMsg* _msg = new GameMsg((GameMsg::MSG_TYPE)id, msg_substring);

    /*�жϺ����Ƿ��б���*/
    pret->_multi_msg.push_back(_msg);
    //�����Ѵ�����
    last_message.erase(0, 8 + length);

  }
 
  return pret;
}

std::string* GameProtocol::response2raw(UserData& _oUserData) {
  /*��Ϸ��Ϣ(GameMsg)--->�ֽ���*/
  std::string* pret = new std::string();

  GET_REF2DATA(GameMsg, data, _oUserData);

  std::string _msg_byte = data.Serielize();
  //ƴ����Ϣ���Ⱥ���Ϣ����
  unsigned int length = _msg_byte.size();
  unsigned int id = data.m_msg_type;

  //׷�ӳ���
  pret->push_back(length & 0xff);
  pret->push_back((length >> 8) & 0xff);
  pret->push_back((length >> 16) & 0xff);
  pret->push_back((length >> 24) & 0xff);

  //׷��ID
  pret->push_back(id & 0xff);
  pret->push_back((id >> 8) & 0xff);
  pret->push_back((id >> 16) & 0xff);
  pret->push_back((id >> 24) & 0xff);

  //׷����Ϣ����
  pret->append(_msg_byte);

  return pret;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg) {
  return pGameRole;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes) {
  return pGameChannel;
}
