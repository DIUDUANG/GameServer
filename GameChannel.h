#ifndef __GAMESERVER_GAMECHANNEL_H__
#define __GAMESERVER_GAMECHANNEL_H__
#include "zinx.h"
#include "ZinxTCP.h"
class GameChannel : public ZinxTcpData {
 public:
  GameChannel(int _fd);
  ~GameChannel();

  void BindProtocol(Iprotocol* _pProtocol);

 private:
  // Í¨¹ý ZinxTcpData ¼Ì³Ð
  virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;

  Iprotocol* pGameProtocol;
};

#endif

