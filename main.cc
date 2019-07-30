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

//创建守护进程
void daemonlize();

int main(int argc, char** argv) {
  daemonlize();

  //加载随机姓名文件
  if (false == NamePool::GetInstance().LoadFile()) {
    printf("Load NameFile failed...\n");
    return -1;
  }


  //初始化框架
  bool retval = ZinxKernel::ZinxKernelInit();
  if (false == retval) {
    printf("KernelInit failed...\n");
    return -1;
  }

  //添加监听通道
  retval = ZinxKernel::Zinx_Add_Channel(
      *(new ZinxTCPListen(12306, new GameChannelFact())));
  if (false == retval) {
    printf("Zinx_Add_Channel failed...\n");
    return -2;
  }

  ZinxKernel::Zinx_Add_Channel(*(new TimerChannel));

  //运行
  ZinxKernel::Zinx_Run();

  //结束框架
  ZinxKernel::ZinxKernelFini();

  return 0;
}

//创建守护进程
void daemonlize() {
  //fork创建子进程
  int pid = fork();
  if (pid < 0) {
    exit(-1);
  }

  else if (pid > 0) {
    //退出父进程
    exit(0);
  }

  else if (0 == pid) {
  //设置会话id
   setsid();

  //重定向文件描述符 0 1 2
   int null_fd = open("/dev/null", O_RDWR);
   if (null_fd >= 0) {
     dup2(null_fd, STDIN_FILENO);
     dup2(null_fd, STDOUT_FILENO);
     dup2(null_fd, STDERR_FILENO);

     close(null_fd);
   }

  }


  /*进程监控*/
  //循环fork--->父进程wait--->子进程执行游戏业务
  while (1) {
    int iPid = fork();
    if (iPid < 0) {
      exit(-1);
    }

    if (iPid > 0) {
      int status;
      wait(&status);
      //若子进程正常退出,则不再fork新的子进程
      if (0 == status) {
        exit(0);
      }
    }

    if (0 == iPid) {
      break;
    }
  }
}