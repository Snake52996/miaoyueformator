#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <queue>
#include <windows.h>
#include <io.h>
//#include <openssl/md5.h>
#include "src/md5_encode.h"
#include "src/file_path_operator.h"

using namespace std;

void listFiles(const string& dir);
void operateTargetFile(const string& target_path);

string CalMD5(const string& resource){
  /*
  string result_md5 = "";
  unsigned char data[110];
  unsigned char md[16];
  char tmp[3] = {'\0'};
  char buf[33] = {'\0'};
  for(int i = 0; i < resource.size(); ++i) data[i] = resource[i];
  data[resource.size()] = '\0';
  MD5(data, resource.size() - 1, md);
  for(int i = 0; i < 16; ++i){
    sprintf(tmp, "%2.2x", md[i]);
    result_md5 = result_md5 + tmp;
  }
  return result_md5;
  */
  MD5 md5(resource);
  string result = md5.md5();
  return result;
}

int main()
{
    char temp[200];
    cout << "The root directory of your comic please: ";
    cin.getline(temp, 200);
    string dir = temp;
    //修正路径的格式保证一致
    for(int i = 0;i < dir.size(); ++i) if(dir[i] == '\\') dir[i] = '/';

    //cout<<"Tell me where you want to save it. Please make sure ";

    //TODO: 获取目标地址并处理

    listFiles(dir);
    return 0;
}

void listFiles(const string& dir)
{
    string dirNew = dir + "/*.*";
    /*
    strcpy(dirNew, dir);
    strcat(dirNew, "/*.*");    // 在目录后面加上"\\*.*"进行第一次搜索
    */

    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(dirNew.c_str(), &findData);
    if (handle == -1)        // 检查是否成功
        return;

    do
    {

        if (findData.attrib & _A_SUBDIR)
        {
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;

            //cout << findData.name << "\t<dir>\n";

            // 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
            //cout<<findData.name<<endl;
            dirNew = dir + findData.name;
            /*
            strcpy(dirNew, dir);
            strcat(dirNew, "/");
            strcat(dirNew, findData.name);
            */

            listFiles(dirNew);
        }
        else
        {

            //cout << findData.name << "\t" << findData.size << " bytes.\n";
            string extension = GetExtension(findData.name);
            if(extension == ".txt"){
              //cout<<dir<<endl<<findData.name<<endl;
              dirNew = dir + "/" + findData.name;
              operateTargetFile(dirNew);
            }
        }

    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // 关闭搜索句柄
}

void operateTargetFile(const string& target_path){
  cout<<"try to open "<<target_path<<endl;
  ifstream fin(target_path.c_str());
  queue<string> waiting_for_operate;
  string target;
  string title;
  char c;
  bool file_end = false;
  bool record = false;  //表示是否(开始)需要记录
  bool is_title = false;  //当前值是否为标题
  while(!file_end)
  {
    //这里理应是个函数,但是没有找到优美的封装方式
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
    if(is_title){
      title = target;
      is_title = false;
    }
    if(target == "title") is_title = true;
    //TODO: 修正中文乱码问题
  }
  //=================Debug====================
  while(!waiting_for_operate.empty()){
    cout<<waiting_for_operate.front()<<endl;
    waiting_for_operate.pop();
  }
  //==========================================

  //TODO: 子目录的创建，文件的复制

  fin.close();
  return;
}
