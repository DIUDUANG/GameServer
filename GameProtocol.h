#ifndef __GAMESERVER_GAMEPROTOCOL_H__
#define __GAMESERVER_GAMEPROTOCOL_H__
#include <string>
#include "zinx.h"

class GameProtocol : public Iprotocol {
 public:
   GameProtocol();
   ~GameProtocol();

   void BindChannel(Ichannel* _pChannel);
   void BindRole(Irole* _pRole);

 private:
  // ͨ�� Iprotocol �̳�
  virtual UserData* raw2request(std::string _szInput) override;
  virtual std::string* response2raw(UserData& _oUserData) override;
  virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
  virtual Ichannel* GetMsgSender(BytesMsg& _oBytes) override;

  Ichannel* pGameChannel;  //�󶨵�ͨ������
  Irole* pGameRole;        //�󶨵Ľ�ɫ����
  std::string last_message;//��һ�α���

};


#endif

