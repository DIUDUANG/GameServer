#include "TimeoutDeliver.h"
#include "TimerChannel.h"
#include "TimeoutTask.h"

TimeoutDeliver* TimeoutDeliver::m_single = NULL;

IZinxMsg* TimeoutDeliver::InternelHandle(IZinxMsg& _oInput) {
  GET_REF2DATA(BytesMsg, timeout_count, _oInput);

  /*ǿת8���ֽڵ��׵�ַ�޷���������ָ��*/
  unsigned long* pcont = (unsigned long*)timeout_count.szData.data();

  for (int i = 0; i < *pcont; i++)
  {
    std::list<TimeoutTask*> tmp_list;
    /*������ǰ�̶�����Ӧ��list��������1*/
    for (auto itr = m_wheel[cur_pos].begin(); itr != m_wheel[cur_pos].end(); )
    {
      /*��������Ϊ0�ˣ���Ҫ���ô����������ü���*/
      if ((*itr)->m_count <= 0)
      {
        /*��ʱ�ˣ��ݴ汾�γ�ʱ������ڵ�*/
        tmp_list.push_back((*itr));
        /*Ҫժ��,����������*/
        itr = m_wheel[cur_pos].erase(itr);

      }
      else
      {
        /*������*/
        (*itr)->m_count--;
        itr++;
      }
    }
    /*���ó�ʱ����Ĵ�������������ӳ�ʱ����ʱ����*/
    for (auto ptask : tmp_list)
    {
      ptask->ProcTimeout();
      RegisterTask(ptask->time_out, ptask);
    }
    /*�̶�����ƶ�һ��*/
    cur_pos++;
    cur_pos %= m_wheel.size();
    /*ѭ����ʱ������*/
  }

  /*û����һ������*/
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
  /*�����������ڸ���*/
  int dest_grid = _sec % m_wheel.size() + cur_pos;
  dest_grid %= m_wheel.size();
  /*����ʣ�����*/
  _task->m_count = _sec / m_wheel.size();
  _task->time_out = _sec;

  /*��Ӳ���_task���󵽶�Ӧ���ӵ�list��*/
  m_wheel[dest_grid].push_back(_task);
}

void TimeoutDeliver::UnregisterTask(TimeoutTask* _task) {
  /*�������п̶�*/
  for (auto& grid : m_wheel)
  {
    bool find = false;
    /*�����ÿ̶ȶ�Ӧ��list---��ɾ��*/
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
  /*���δ���ʱ���ֵ����п̶�*/
  for (int i = 0; i < 10; i++)
  {
    std::list<TimeoutTask*> tmp;
    m_wheel.push_back(tmp);
  }
}

TimeoutDeliver::~TimeoutDeliver() {
}
