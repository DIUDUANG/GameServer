#ifndef __GAMESERVER_AOI_WORLD_H__yyp
#define __GAMESERVER_AOI_WORLD_H__yyp
#define DISALLOW_COPY_AND_ASSIGN(Typename) \
        Typename(const Typename&);  \
        void operator=(const Typename&)
#include <vector>
#include "AOI_Grid.h"

class AOI_World {
 public:
   //����
   AOI_World(int _x_begin,
     int _x_end,
     int _y_begin,
     int _y_end,
     int _x_count,
     int _y_count);
   //����
   ~AOI_World();

   //��ȡĳ�����Χ���
   std::list<AOI_Player*> GetSrdPlayers(AOI_Player* _player);
   //�����ҵ���Ϸ����
   bool AddPlayer(AOI_Player* _player);
   //ժ�����
   bool DelPlayer(AOI_Player* _player);

 private:
   //��ȡx��������
   int GetXWidth();
   //��ȡy��������
   int GetYWidth();

  //������ɸ��Ӷ���
  std::vector<AOI_Grid> m_grids;

  //���ε�ͼ�Ĳ���
  int x_begin;  //x����ʼ����
  int x_end;    //x���������
  int y_begin;  //y����ʼ����
  int y_end;    //y���������

  //��ͼ�ĸ�����
  int x_count;  //x��ĸ�����
  int y_count;  //y��ĸ�����

  //��ֹ���ɿ�����������ظ�ֵ�����
  DISALLOW_COPY_AND_ASSIGN(AOI_World);
};

#endif