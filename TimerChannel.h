#ifndef __GAMESERVER_TIMERCHANNEL_H__
#define __GAMESERVER_TIMERCHANNEL_H__
#include "zinx.h"

class TimerChannel : public Ichannel {
 public:
  TimerChannel();
  virtual ~TimerChannel();

  // Í¨¹ý Ichannel ¼Ì³Ð
  virtual bool Init() override;
  virtual bool ReadFd(std::string& _input) override;
  virtual bool WriteFd(std::string& _output) override;
  virtual void Fini() override;
  virtual int GetFd() override;
  virtual std::string GetChannelInfo() override;
  virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;

 private:
  int m_fd;
};

#endif