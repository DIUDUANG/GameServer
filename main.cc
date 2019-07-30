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
#include "NamePool.h"
#include "FirstName.h"
#include "TimerChannel.h"

//�����ػ�����
void daemonlize();

int main(int argc, char** argv) {
  daemonlize();

  //������������ļ�
  if (false == NamePool::GetInstance().LoadFile()) {
    printf("Load NameFile failed...\n");
    return -1;
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

  ZinxKernel::Zinx_Add_Channel(*(new TimerChannel));

  //����
  ZinxKernel::Zinx_Run();

  //�������
  ZinxKernel::ZinxKernelFini();

  return 0;
}

//�����ػ�����
void daemonlize() {
  //fork�����ӽ���
  int pid = fork();
  if (pid < 0) {
    exit(-1);
  }

  else if (pid > 0) {
    //�˳�������
    exit(0);
  }

  else if (0 == pid) {
  //���ûỰid
   setsid();

  //�ض����ļ������� 0 1 2
   int null_fd = open("/dev/null", O_RDWR);
   if (null_fd >= 0) {
     dup2(null_fd, STDIN_FILENO);
     dup2(null_fd, STDOUT_FILENO);
     dup2(null_fd, STDERR_FILENO);

     close(null_fd);
   }

  }


  /*���̼��*/
  //ѭ��fork--->������wait--->�ӽ���ִ����Ϸҵ��
  while (1) {
    int iPid = fork();
    if (iPid < 0) {
      exit(-1);
    }

    if (iPid > 0) {
      int status;
      wait(&status);
      //���ӽ��������˳�,����fork�µ��ӽ���
      if (0 == status) {
        exit(0);
      }
    }

    if (0 == iPid) {
      break;
    }
  }
}