#ifndef __GAMESERVER_AOI_PLAYER_H__
#define __GAMESERVER_AOI_PLAYER_H__
#include <string>

class AOI_Player {
public:
   virtual int get_x() = 0;   //��ȡ��Һ�����
   virtual int get_y() = 0;   //��ȡ���������
};

#endif