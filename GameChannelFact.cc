#include "GameChannelFact.h"
#include "GameChannel.h"
#include "GameProtocol.h"
#include "GameRole.h"

ZinxTcpData* GameChannelFact::CreateTcpDataChannel(int _fd) {
  //����GameChannel������
  GameChannel* pGameChannel = new GameChannel(_fd);

  //����Э�����(���ڰ�)
  GameProtocol* pGameProtocol = new GameProtocol();
  
  //������ɫ����(���ڰ�)
  GameRole* pGameRole = new GameRole();

  //��Э�����󶨵�ͨ������
  pGameChannel->BindProtocol(pGameProtocol);

  //��ͨ������󶨵�Э�����
  pGameProtocol->BindChannel(pGameChannel);

  //����ɫ����󶨵�Э�����
  pGameProtocol->BindRole(pGameRole);

  //��Э�����󶨵���ɫ����
  pGameRole->BindProtocol(pGameProtocol);

  //��role�����protocol������ӵ�kernel��
  ZinxKernel::Zinx_Add_Proto(*pGameProtocol);
  ZinxKernel::Zinx_Add_Role(*pGameRole);

  return pGameChannel;
}
