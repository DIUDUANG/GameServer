#ifndef __GAMESERVER_EXITFRAMEWORKTASK_H__
#define __GAMESERVER_EXITFRAMEWORKTASK_H__
#include "TimeoutTask.h"

class ExitFrameworkTask : public TimeoutTask {
  // ͨ�� TimeoutTask �̳�
  virtual void ProcTimeout() override;
};

#endif