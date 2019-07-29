#ifndef __GAMESERVER_MULTIMSG_H__
#define __GAMESERVER_MULTIMSG_H__
#include <cstdio>
#include <iostream>
#include <string>
#include <list>
#include "zinx.h"
#include "GameMsg.h"

class MultiMsg : public UserData {
 public:
   ~MultiMsg();

   std::list<GameMsg*> _multi_msg;

};

#endif
