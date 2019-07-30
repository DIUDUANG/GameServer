#ifndef __GAMESERVER_EXITFRAMEWORKTASK_H__
#define __GAMESERVER_EXITFRAMEWORKTASK_H__
#include "TimeoutTask.h"

class ExitFrameworkTask : public TimeoutTask {
  // Í¨¹ý TimeoutTask ¼Ì³Ð
  virtual void ProcTimeout() override;
};

#endif