#ifndef __GAMESERVER_TIMEOUTTASK_H__
#define __GAMESERVER_TIMEOUTTASK_H__

class TimeoutTask {
 public:
  virtual void ProcTimeout() = 0;
  int m_count = 0;
  int time_out = 0;
};

#endif