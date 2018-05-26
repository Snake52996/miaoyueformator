#include <iostream>
#include <fstream>
#include <cstring>
#include <io.h>
#include "src/file_path_operator.h"

using namespace std;

void listFiles(const string& dir);
void operateTargetFile(const string& target_path);

int main()
{
    char temp[200];
    cout << "Enter a directory: ";
    cin.getline(temp, 200);
    string dir = temp;

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
            dirNew = dir + "/" + findData.name;
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
              //cout<<findData.name<<endl;
              dirNew = dir + "/" + findData.name;
              operateTargetFile(dirNew);
            }
        }

    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // 关闭搜索句柄
}

void operateTargetFile(const string& target_path){
  ifstream fin(target_path.c_str());
}
