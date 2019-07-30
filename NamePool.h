#ifndef __GAMESERVER_NAMEPOOL_H__
#define __GAMESERVER_NAMEPOOL_H__
#include "FirstName.h"

//���������
class NamePool {
 public:
  //��ȡ�������
   std::string GetName();
  //�ͷ�����
   void ReleaseName(std::string name);
  //���������ļ�
   bool LoadFile();
   //��ȡ����ʵ��
   static NamePool& GetInstance();

 private:
   NamePool();
   virtual ~NamePool();

   static NamePool name_pool;
  std::vector<FirstName_Vector> m_pool;
};

#endif