#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <queue>
#include <windows.h>
#include <io.h>
#include <thread>
#include <mutex>
#include "src/md5_encode.h"
#include "src/file_path_operator.h"
#include "src/string_counter.h"
#include "src/sweet_printer.h"
#include "src/string_operator.h"

using namespace std;

void listFiles(const string& dir);
void operateTargetFile(const string& target_path, const string& target_file);

//Bug: 在结果的第一个位置上会出现一个?
string UTF8_To_string(const string& str){
  int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
  wchar_t * pwBuf = new wchar_t[nwLen + 1];
  memset(pwBuf, 0, nwLen * 2 + 2);
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
  int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
  char * pBuf = new char[nLen + 1];
  memset(pBuf, 0, nLen + 1);
  WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
  string retStr = pBuf;
  delete []pBuf;
  delete []pwBuf;
  pBuf = NULL;
  pwBuf = NULL;
  return retStr;
}

string result_path = "";  //用于存储保存结果的路径
int checker = 0;
mutex Rlock;

//计算MD5
string CalMD5(const string& resource){
  MD5 md5(resource);
  string result = md5.md5();
  return result;
}

int main()
{
    char temp[200];
    PrintStringAndEndlineSlower("The root directory of your comic please: ", 25);
    cin.getline(temp, 200);
    string dir = temp;
    //修正路径的格式保证一致
    ChangeBackSlantIntoSlant(dir);

    PrintStringAndEndlineSlower("Tell me where you want to save it: ", 25);
    cin.getline(temp, 200);
    result_path = temp;
    ChangeBackSlantIntoSlant(result_path);

    //unamed.initialize(2);
    listFiles(dir);
    PrintStringAndEndlineSlower("We have finished our work. Here we go!", 25);
    string command = "explorer.exe " + result_path + "/";
    ChangeSlantIntoBackSlant(command);
    system(command.c_str());  //打开资源管理器显示结果
    return 0;
}

void listFiles(const string& dir)
{
    string dirNew = dir + "/*.*";  //连接用于遍历的文件名
    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(dirNew.c_str(), &findData);
    if (handle == -1)        // 检查是否成功
        return;
    do
    {

        if (findData.attrib & _A_SUBDIR)
        {
            //忽略当前目录和上层目录
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;
            dirNew = dir + findData.name;
            listFiles(dirNew);  //发现子目录，进入查找
        }
        else
        {
            string extension = GetExtension(findData.name);
            if(extension == ".txt"){
              dirNew = dir + "/" + findData.name;
              thread t{operateTargetFile, dir, dirNew};  //找到了.txt的描述文件,开始操作
              t.detach();
            }
        }

    } while (_findnext(handle, &findData) == 0);
    while(checker) Sleep(500);
    _findclose(handle);    // 关闭搜索句柄
}

void operateTargetFile(const string& target_path, const string& target_file){
  //ClearCli();  //清屏
  //PrintStringSlower("Try to open ", 25);
  //PrintStringAndEndlineSlower(target_file.c_str(), 25);
  Rlock.lock();
  checker += 1;
  Rlock.unlock();
  ifstream fin(target_file.c_str());
  fin>>noskipws;
  queue<string> waiting_for_operate;
  string target;
  string title = "";
  char c;
  bool file_end = false;
  bool record = false;  //表示是否(开始)需要记录
  while(!file_end)
  {
    //这里理应是个函数,但是没有找到优美的封装方式
    //读入双引号括起来的信息
    target = "";
    fin>>c;
    for(; c != '\"' && c != '}'; fin>>c);
    if(c == '}'){
      file_end = 1;
      break;
    }
    fin>>c;
    for(; c!= '\"'; fin>>c) target.push_back(c);
    //虽然这样更不优美，但是就先这样吧
    //cout<<target<<endl;
    if(record){
      if(target != "page"){
        waiting_for_operate.push(CalMD5(target));
      }
      else record = false;
    }
    if(target == "img") record = true;
    if(target == "title"){
      fin>>c;
      fin>>c;
      fin>>c;
      for(;c != '\"'; fin>>c) title.push_back(c);
      title = UTF8_To_string(title);
      RemovePrefixAndSuffixSpace(title);  //去除前后缀空格
      //cout<<title<<endl;
      break;
    }
  }

  //创建子目录
  Rlock.lock();
  string command = "md \"" + result_path + "/" + title + "/\"";
  Rlock.unlock();
  string_counter filename;
  filename.initialize(3);
  Rlock.lock();
  system(command.c_str());
  Rlock.unlock();
  while(!waiting_for_operate.empty()){
    //复制文件
    Rlock.lock();
    command = "copy \"" + target_path + "/" + waiting_for_operate.front() + "\" \"" + result_path + "/" + title + "/" + filename.get_counter() + ".jpg\"";
    Rlock.unlock();
    ChangeSlantIntoBackSlant(command);
    Rlock.lock();
    system(command.c_str());
    Rlock.unlock();
    filename.add_one();
    waiting_for_operate.pop();
  }
  //PrintStringSlower("Finished ", 25);
  //PrintStringAndEndlineSlower(target_file.c_str(),25);
  //cout<<endl;
  fin.close();
  //ClearCli();
  Rlock.lock();
  checker -= 1;
  Rlock.unlock();
  return;
}
