#include "ExitFrameworkTask.h"
#include "zinx.h"

void ExitFrameworkTask::ProcTimeout() {
  ZinxKernel::Zinx_Exit();
}
