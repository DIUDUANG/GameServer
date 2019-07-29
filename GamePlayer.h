#ifndef __GAMESERVER_GAMEPLAYER_H__
#define __GAMESERVER_GAMEPLAYER_H__
#define DISALLOW_COPY_AND_ASSIGN(Typename) \
        Typename(const Typename&);  \
        void operator=(const Typename&)
#include <string>
#include "AOI_Player.h"

//��Ϸ�����,�̳���AOI_PLAYER
class GamePlayer : public AOI_Player {
 public:
  //���캯��
  GamePlayer(int _x, int _y, std::string _name);
 // ͨ�� AOI_Player �̳�
  //��ȡx������
  virtual int get_x() override;
  //��ȡy������
  virtual int get_y() override;
  
  std::string name;   //�ǳ�

 private:
  int x;              //������
  int y;              //������


  //��ֹ���ɿ�����������ظ�ֵ�����
  DISALLOW_COPY_AND_ASSIGN(GamePlayer);
};

#endif