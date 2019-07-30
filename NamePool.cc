#include "NamePool.h"
#include <fstream>
#include <random>

namespace {
std::default_random_engine randSrc(time(NULL));
}

NamePool NamePool::name_pool;

std::string NamePool::GetName() {
  //姓名池被取干
  if (m_pool.size() <= 0) {
    return std::string(reinterpret_cast<char*>(randSrc()));
  }

  //随机取出姓
  int first_index = randSrc() % m_pool.size();
  FirstName_Vector& first_vector = m_pool[first_index];
  std::string first_name = first_vector.m_first_name;

  //随机取出名
  int name_index = randSrc() % first_vector.m_last_name_vector.size();
  std::string last_name = first_vector.m_last_name_vector[name_index];
  //摘除名字
  first_vector.m_last_name_vector.erase(first_vector.m_last_name_vector.begin() + name_index);
  //若名字摘完--->摘掉姓
  if (first_vector.m_last_name_vector.size() <= 0) {
    m_pool.erase(m_pool.begin() + first_index);
  }

  //拼接姓名

  return first_name + " " + last_name;
}

void NamePool::ReleaseName(std::string _name) {
  //拆分全名--->姓 + 名
  int space_pos = _name.find(' ');
  std::string first_name = _name.substr(0, space_pos);
  std::string last_name = _name.substr(space_pos + 1, _name.size() - space_pos - 1);
  bool flag = false;//表示有没有找到对应的姓

  //根据姓找到名应该存放的容器
  for (auto first : m_pool) {
    if (first.m_first_name == first_name) {
      first.m_last_name_vector.push_back(last_name);
      flag = true;
      break;
    }
  }

  if (false == flag) {
    //没找到对应的姓--->创建对应的姓名容器对象--->存名
    FirstName_Vector tmp;
    tmp.m_first_name = first_name;
    tmp.m_last_name_vector.push_back(last_name);

    //存到姓名池中
    m_pool.push_back(tmp);
  }

}

bool NamePool::LoadFile() {
  //读名字文件,每行名字存到vector中
  std::ifstream name_file("random_last.txt");
  if (false == name_file.is_open()) {
    return false;
  }

  std::string tmp;
  std::vector<std::string> tmp_last_name;

  while (std::getline(name_file, tmp)) {
    tmp_last_name.push_back(tmp);
  }

  //读姓文件,每行姓都要添加一个姓名池成员
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
