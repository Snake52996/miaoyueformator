#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <locale>
#include <queue>
#include <windows.h>
#include <io.h>
#include "src/md5_encode.h"
#include "src/file_path_operator.h"
#include "src/string_counter.h"
#include "src/sweet_printer.h"
#include "src/string_operator.h"

using namespace std;

void listFiles(const string& dir);
void operateTargetFile(const string& target_path, const string& target_file);

string result_path = "";  //���ڴ洢��������·��
string_counter unamed;  //�������������½�

//����MD5
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
    //����·���ĸ�ʽ��֤һ��
    ChangeBackSlantIntoSlant(dir);

    PrintStringAndEndlineSlower("Tell me where you want to save it: ", 0);
    cin.getline(temp, 200);
    result_path = temp;
    ChangeBackSlantIntoSlant(result_path);

    unamed.initialize(2);
    listFiles(dir);
    /*
    PrintStringAndEndlineSlower("We have finished our work. Here we go!", 25);
    string command = "explorer.exe " + result_path + "/";
    ChangeSlantIntoBackSlant(command);
    system(command.c_str());  //����Դ��������ʾ���
    */
    return 0;
}

void listFiles(const string& dir)
{
    string dirNew = dir + "/*.*";  //�������ڱ������ļ���
    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(dirNew.c_str(), &findData);
    if (handle == -1)        // ����Ƿ�ɹ�
        return;
    do
    {

        if (findData.attrib & _A_SUBDIR)
        {
            //���Ե�ǰĿ¼���ϲ�Ŀ¼
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;
            dirNew = dir + findData.name;
            listFiles(dirNew);  //������Ŀ¼���������
        }
        else
        {
            string extension = GetExtension(findData.name);
            if(extension == ".txt"){
              dirNew = dir + "/" + findData.name;
              operateTargetFile(dir, dirNew);  //�ҵ���.txt�������ļ�,��ʼ����
            }
        }

    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // �ر��������
}

void operateTargetFile(const string& target_path, const string& target_file){
  ClearCli();  //����
  PrintStringSlower("Try to open ", 0);
  PrintStringAndEndlineSlower(target_file.c_str(), 0);
  ifstream fin(target_file.c_str());
  fin>>noskipws;
  queue<string> waiting_for_operate;
  string target;
  string title = "";
  char c;
  bool file_end = false;
  bool record = false;  //��ʾ�Ƿ�(��ʼ)��Ҫ��¼
  while(!file_end)
  {
    //������Ӧ�Ǹ�����,����û���ҵ������ķ�װ��ʽ
    //����˫��������������Ϣ
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
    if(target == "title"){
      for(; c != '}'; fin>>c) if((c >= '0' && c <= '9') || c == ' ' || c == '.' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) title.push_back(c);
      //cout<<"title = "<<title<<endl;
      RemovePrefixAndSuffixSpace(title);  //ȥ��ǰ��׺�ո�
      if(title.size() == 0){
        title = "unamed" + unamed.get_counter();
        unamed.add_one();
      }
      break;
    }
    //(NOT)TODO: ����������������
  }
  /*/=================Debug=====================
  while(!waiting_for_operate.empty()){
    cout<<waiting_for_operate.front()<<endl;
    waiting_for_operate.pop();
  }
  //==========================================*/

  //������Ŀ¼
  string command = "md \"" + result_path + "/" + title + "/\"";
  //cout<<command<<endl;
  //���ַ�����ʶ�ļ���
  string_counter filename;
  filename.initialize(3);
  system(command.c_str());
  while(!waiting_for_operate.empty()){
    //�����ļ�
    command = "copy \"" + target_path + "/" + waiting_for_operate.front() + "\" \"" + result_path + "/" + title + "/" + filename.get_counter() + ".jpg\"";
    ChangeSlantIntoBackSlant(command);
    //cout<<command<<endl;
    system(command.c_str());
    filename.add_one();
    waiting_for_operate.pop();
  }
  PrintStringSlower("Finished ", 0);
  PrintStringAndEndlineSlower(target_file.c_str(),0);
  cout<<endl;
  fin.close();
  ClearCli();
  return;
}
