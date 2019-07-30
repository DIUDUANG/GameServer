#include "NamePool.h"
#include <fstream>
#include <random>

namespace {
std::default_random_engine randSrc(time(NULL));
}

NamePool NamePool::name_pool;

std::string NamePool::GetName() {
  //�����ر�ȡ��
  if (m_pool.size() <= 0) {
    return std::string(reinterpret_cast<char*>(randSrc()));
  }

  //���ȡ����
  int first_index = randSrc() % m_pool.size();
  FirstName_Vector& first_vector = m_pool[first_index];
  std::string first_name = first_vector.m_first_name;

  //���ȡ����
  int name_index = randSrc() % first_vector.m_last_name_vector.size();
  std::string last_name = first_vector.m_last_name_vector[name_index];
  //ժ������
  first_vector.m_last_name_vector.erase(first_vector.m_last_name_vector.begin() + name_index);
  //������ժ��--->ժ����
  if (first_vector.m_last_name_vector.size() <= 0) {
    m_pool.erase(m_pool.begin() + first_index);
  }

  //ƴ������

  return first_name + " " + last_name;
}

void NamePool::ReleaseName(std::string _name) {
  //���ȫ��--->�� + ��
  int space_pos = _name.find(' ');
  std::string first_name = _name.substr(0, space_pos);
  std::string last_name = _name.substr(space_pos + 1, _name.size() - space_pos - 1);
  bool flag = false;//��ʾ��û���ҵ���Ӧ����

  //�������ҵ���Ӧ�ô�ŵ�����
  for (auto first : m_pool) {
    if (first.m_first_name == first_name) {
      first.m_last_name_vector.push_back(last_name);
      flag = true;
      break;
    }
  }

  if (false == flag) {
    //û�ҵ���Ӧ����--->������Ӧ��������������--->����
    FirstName_Vector tmp;
    tmp.m_first_name = first_name;
    tmp.m_last_name_vector.push_back(last_name);

    //�浽��������
    m_pool.push_back(tmp);
  }

}

bool NamePool::LoadFile() {
  //�������ļ�,ÿ�����ִ浽vector��
  std::ifstream name_file("random_last.txt");
  if (false == name_file.is_open()) {
    return false;
  }

  std::string tmp;
  std::vector<std::string> tmp_last_name;

  while (std::getline(name_file, tmp)) {
    tmp_last_name.push_back(tmp);
  }

  //�����ļ�,ÿ���ն�Ҫ���һ�������س�Ա
  std::ifstream first_file("random_first.txt");
  if (false == first_file.is_open()) {
    return false;
  }
  
  while (std::getline(first_file, tmp)) {
    FirstName_Vector tmp_vector;
    tmp_vector.m_first_name = tmp;
    tmp_vector.m_last_name_vector = tmp_last_name;
    m_pool.push_back(tmp_vector);
  }

  return true;
}

NamePool& NamePool::GetInstance() {
  return name_pool;
}

NamePool::NamePool() {

}

NamePool::~NamePool() {
}
