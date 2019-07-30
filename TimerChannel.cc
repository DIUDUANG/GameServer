#include "TimerChannel.h"
#include <sys/timerfd.h>
#include <stdio.h>
#include "TimeoutDeliver.h"

TimerChannel::TimerChannel() : m_fd(-1) {

}

TimerChannel::~TimerChannel() {
}

bool TimerChannel::Init() {
  bool bRet = false;
  int fd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (fd >= 0)
  {
    m_fd = fd;
    struct itimerspec period = { {1,0},{1,0} };
    //¿ªÆô¶¨Ê±Æ÷
    if (0 == timerfd_settime(m_fd, 0, &period, NULL))
    {
      bRet = true;
    }
    else
    {
      close(m_fd);
      m_fd = -1;
    }

  }
  return bRet;
}

bool TimerChannel::ReadFd(std::string& _input) {
  bool bRet = false;
  char buff[8];
  if (sizeof(buff) == read(m_fd, buff, sizeof(buff)))
  {
    _input.append(buff, sizeof(buff));
    bRet = true;
  }
  return bRet;
}

bool TimerChannel::WriteFd(std::string& _output) {
  return false;
}

void TimerChannel::Fini() {
  if (m_fd >= 0)
  {
    close(m_fd);
  }
}

int TimerChannel::GetFd() {
  return m_fd;
}

std::string TimerChannel::GetChannelInfo() {
  return "timerfd" + m_fd;
}

AZinxHandler* TimerChannel::GetInputNextStage(BytesMsg& _oInput) {
  return TimeoutDeliver::GetInstance();
}
