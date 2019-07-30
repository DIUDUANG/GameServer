#ifndef __GAMESERVER_TIMEOUTDELIVER_H__
#define __GAMESERVER_TIMEOUTDELIVER_H__
#include <vector>
#include <list>
#include "zinx.h"
#include "TimeoutTask.h"

class TimeoutDeliver : public AZinxHandler {
 public:
  // 通过 AZinxHandler 继承
  virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
  virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;

  //获取单例对象指针
  static TimeoutDeliver* GetInstance();
  //注册任务
  void RegisterTask(int _sec, TimeoutTask* _task);
  //反注册任务
  void UnregisterTask(TimeoutTask* _task);

 private:
  TimeoutDeliver();
  virtual ~TimeoutDeliver();

  static TimeoutDeliver* m_single;  //单例对象指针
  int cur_pos;                      //当前指向刻度
  std::vector<std::list<TimeoutTask*>> m_wheel;
};

#endif