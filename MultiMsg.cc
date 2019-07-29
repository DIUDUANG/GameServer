#include "MultiMsg.h"

MultiMsg::~MultiMsg() {
  for (auto r : _multi_msg) {
    delete r;
  }
}
