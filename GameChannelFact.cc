#include "GameChannelFact.h"
#include "GameChannel.h"
#include "GameProtocol.h"
#include "GameRole.h"

ZinxTcpData* GameChannelFact::CreateTcpDataChannel(int _fd) {
  //创建GameChannel并返回
  GameChannel* pGameChannel = new GameChannel(_fd);

  //创建协议对象(用于绑定)
  GameProtocol* pGameProtocol = new GameProtocol();
  
  //创建角色对象(用于绑定)
  GameRole* pGameRole = new GameRole();

  //将协议对象绑定到通道对象
  pGameChannel->BindProtocol(pGameProtocol);

  //将通道对象绑定到协议对象
  pGameProtocol->BindChannel(pGameChannel);

  //将角色对象绑定到协议对象
  pGameProtocol->BindRole(pGameRole);

  //将协议对象绑定到角色对象
  pGameRole->BindProtocol(pGameProtocol);

  //将role对象和protocol对象添加到kernel中
  ZinxKernel::Zinx_Add_Proto(*pGameProtocol);
  ZinxKernel::Zinx_Add_Role(*pGameRole);

  return pGameChannel;
}
