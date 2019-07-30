#ifndef __GAMESERVER_NAMEPOOL_H__
#define __GAMESERVER_NAMEPOOL_H__
#include "FirstName.h"

//随机姓名池
class NamePool {
 public:
  //获取随机姓名
   std::string GetName();
  //释放姓名
   void ReleaseName(std::string name);
  //加载姓名文件
   bool LoadFile();
   //获取单例实例
   static NamePool& GetInstance();

 private:
   NamePool();
   virtual ~NamePool();

   static NamePool name_pool;
  std::vector<FirstName_Vector> m_pool;
};

#endif