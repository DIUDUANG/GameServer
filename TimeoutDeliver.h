#ifndef __GAMESERVER_TIMEOUTDELIVER_H__
#define __GAMESERVER_TIMEOUTDELIVER_H__
#include <vector>
#include <list>
#include "zinx.h"
#include "TimeoutTask.h"

class TimeoutDeliver : public AZinxHandler {
 public:
  // ͨ�� AZinxHandler �̳�
  virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
  virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;

  //��ȡ��������ָ��
  static TimeoutDeliver* GetInstance();
  //ע������
  void RegisterTask(int _sec, TimeoutTask* _task);
  //��ע������
  void UnregisterTask(TimeoutTask* _task);

 private:
  TimeoutDeliver();
  virtual ~TimeoutDeliver();

  static TimeoutDeliver* m_single;  //��������ָ��
  int cur_pos;                      //��ǰָ��̶�
  std::vector<std::list<TimeoutTask*>> m_wheel;
};

#endif