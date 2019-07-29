#include "GameChannel.h"

GameChannel::GameChannel(int _fd) : ZinxTcpData(_fd), pGameProtocol (NULL) {
}

GameChannel::~GameChannel() {
  //���ӶϿ�--->ժ��Э�����-->���ͷ�
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
