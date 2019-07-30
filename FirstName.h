#ifndef __GAMESERVER_FIRSTNAME_H__
#define __GAMESERVER_FIRSTNAME_H__
#include <vector>
#include <string>

//姓氏容器
class FirstName_Vector {
 public:
  std::string m_first_name;                     //姓氏
  std::vector<std::string> m_last_name_vector;  //该姓所对应的名字容器
};

#endif
