#include "GameChannel.h"

GameChannel::GameChannel(int _fd) : ZinxTcpData(_fd), pGameProtocol (NULL) {
}

GameChannel::~GameChannel() {
  //连接断开--->摘除协议对象-->并释放
  ZinxKernel::Zinx_Del_Proto(*pGameProtocol);
  delete pGameProtocol;
  pGameProtocol = NULL;

}

void GameChannel::BindProtocol(Iprotocol* _pProtocol) {
  pGameProtocol = _pProtocol;
}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput) {
  return pGameProtocol;
}
