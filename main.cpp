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
    //����·���ĸ�ʽ��֤һ��
    for(int i = 0;i < dir.size(); ++i) if(dir[i] == '\\') dir[i] = '/';

    //cout<<"Tell me where you want to save it. Please make sure ";

    //TODO: ��ȡĿ���ַ������

    listFiles(dir);
    return 0;
}

void listFiles(const string& dir)
{
    string dirNew = dir + "/*.*";
    /*
    strcpy(dirNew, dir);
    strcat(dirNew, "/*.*");    // ��Ŀ¼�������"\\*.*"���е�һ������
    */

    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(dirNew.c_str(), &findData);
    if (handle == -1)        // ����Ƿ�ɹ�
        return;

    do
    {

        if (findData.attrib & _A_SUBDIR)
        {
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;

            //cout << findData.name << "\t<dir>\n";

            // ��Ŀ¼�������"\\"����������Ŀ¼��������һ������
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

    _findclose(handle);    // �ر��������
}

void operateTargetFile(const string& target_path){
  cout<<"try to open "<<target_path<<endl;
  ifstream fin(target_path.c_str());
  queue<string> waiting_for_operate;
  string target;
  string title;
  char c;
  bool file_end = false;
  bool record = false;  //��ʾ�Ƿ�(��ʼ)��Ҫ��¼
  bool is_title = false;  //��ǰֵ�Ƿ�Ϊ����
  while(!file_end)
  {
    //������Ӧ�Ǹ�����,����û���ҵ������ķ�װ��ʽ
    target = "";
    fin>>c;
    for(; c != '\"' && c != '}'; fin>>c);
    if(c == '}'){
      file_end = 1;
      break;
    }
    fin>>c;
    for(; c!= '\"'; fin>>c) target.push_back(c);
    //��Ȼ�����������������Ǿ���������
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
    //TODO: ����������������
  }
  //=================Debug====================
  while(!waiting_for_operate.empty()){
    cout<<waiting_for_operate.front()<<endl;
    waiting_for_operate.pop();
  }
  //==========================================

  //TODO: ��Ŀ¼�Ĵ������ļ��ĸ���

  fin.close();
  return;
}
