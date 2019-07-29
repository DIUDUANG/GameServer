#ifndef __GAMESERVER_GAMECHANNELFACT_H__
#define __GAMESERVER_GAMECHANNELFACT_H__
#include "zinx.h"
#include "ZinxTCP.h"


class GameChannelFact : public IZinxTcpConnFact {
  // Í¨¹ý IZinxTcpConnFact ¼Ì³Ð
  virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};

#endif


