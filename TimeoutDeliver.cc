#include "TimeoutDeliver.h"
#include "TimerChannel.h"
#include "TimeoutTask.h"

TimeoutDeliver* TimeoutDeliver::m_single = NULL;

IZinxMsg* TimeoutDeliver::InternelHandle(IZinxMsg& _oInput) {
  GET_REF2DATA(BytesMsg, timeout_count, _oInput);

  /*强转8个字节的首地址无符号整数的指针*/
  unsigned long* pcont = (unsigned long*)timeout_count.szData.data();

  for (int i = 0; i < *pcont; i++)
  {
    std::list<TimeoutTask*> tmp_list;
    /*遍历当前刻度所对应的list，减计数1*/
    for (auto itr = m_wheel[cur_pos].begin(); itr != m_wheel[cur_pos].end(); )
    {
      /*若计数减为0了，则要调用处理函数，重置计数*/
      if ((*itr)->m_count <= 0)
      {
        /*超时了，暂存本次超时的任务节点*/
        tmp_list.push_back((*itr));
        /*要摘掉,后边重新添加*/
        itr = m_wheel[cur_pos].erase(itr);

      }
      else
      {
        /*减计数*/
        (*itr)->m_count--;
        itr++;
      }
    }
    /*调用超时任务的处理函数，重新添加超时任务到时间轮*/
    for (auto ptask : tmp_list)
    {
      ptask->ProcTimeout();
      RegisterTask(ptask->time_out, ptask);
    }
    /*刻度向后移动一格*/
    cur_pos++;
    cur_pos %= m_wheel.size();
    /*循环超时次数次*/
  }

  /*没有下一个环节*/
  return nullptr;
}

AZinxHandler* TimeoutDeliver::GetNextHandler(IZinxMsg& _oNextMsg) {
  return nullptr;
}

TimeoutDeliver* TimeoutDeliver::GetInstance() {
  if (NULL == m_single) {
    m_single = new TimeoutDeliver();
  }
  return m_single;
}

void TimeoutDeliver::RegisterTask(int _sec, TimeoutTask* _task) {
  /*计算任务所在格子*/
  int dest_grid = _sec % m_wheel.size() + cur_pos;
  dest_grid %= m_wheel.size();
  /*计算剩余计数*/
  _task->m_count = _sec / m_wheel.size();
  _task->time_out = _sec;

  /*添加参数_task对象到对应格子的list里*/
  m_wheel[dest_grid].push_back(_task);
}

void TimeoutDeliver::UnregisterTask(TimeoutTask* _task) {
  /*遍历所有刻度*/
  for (auto& grid : m_wheel)
  {
    bool find = false;
    /*遍历该刻度对应的list---》删除*/
    for (auto ptask : grid)
    {
      if (ptask == _task)
      {
        grid.remove(_task);
        find = true;
        break;
      }
    }
    if (true == find)
    {
      break;
    }
  }
}

TimeoutDeliver::TimeoutDeliver() {
  /*依次创建时间轮的所有刻度*/
  for (int i = 0; i < 10; i++)
  {
    std::list<TimeoutTask*> tmp;
    m_wheel.push_back(tmp);
  }
}

TimeoutDeliver::~TimeoutDeliver() {
}
