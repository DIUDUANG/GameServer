#include <cstdio>
#include <iostream>
#include "zinx.h"
#include "ZinxTCP.h"
#include "GameChannel.h"
#include "GameChannelFact.h"
#include "GameMsg.h"
#include "GameProtocol.h"
#include "GameRole.h"
#include "GamePlayer.h"
#include "AOI_World.h"
#include "AOI_Grid.h"

int main(int argc, char** argv) {

  //������Ϸ����
  AOI_World gameWorld(20, 200, 20, 200, 6, 6);
  GamePlayer p01(20, 20, "dog");
  GamePlayer p02(50, 20, "cat");
  GamePlayer p03(20, 50, "Peroson");
  gameWorld.AddPlayer(&p01);
  gameWorld.AddPlayer(&p02);
  gameWorld.AddPlayer(&p03);

  std::list<AOI_Player*>players = gameWorld.GetSrdPlayers(&p01);
  for (auto r : players) {
    auto p = dynamic_cast<GamePlayer*>(r);
    std::cout << p->name << std::endl;

  }

  //��ʼ�����
  bool retval = ZinxKernel::ZinxKernelInit();
  if (false == retval) {
    printf("KernelInit failed...\n");
    return -1;
  }

  //��Ӽ���ͨ��
  retval = ZinxKernel::Zinx_Add_Channel(
      *(new ZinxTCPListen(12306, new GameChannelFact())));
  if (false == retval) {
    printf("Zinx_Add_Channel failed...\n");
    return -2;
  }

  //����
  ZinxKernel::Zinx_Run();

  //�������
  ZinxKernel::ZinxKernelFini();

  return 0;
}