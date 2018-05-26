#ifndef FILE_PATH_OPERATOR_H_
#define FILE_PATH_OPERATOR_H_
#include<iostream>
#include<cstring>
using namespace std;

string GetExtension(const string& filename){
  string temp, extension = "";
  //cout<<"huaQ\n";
  int i;
  for(i = filename.size() - 1; i >= -1 && filename[i+1] != '.'; --i){
    temp.push_back(filename[i]);
  }
  //cout<<"huaQ2\n";
  if(i != -1){
    for(int j = temp.size() - 1; j >= 0; --j) extension.push_back(temp[j]);
  }
  //cout<<"huaQ3\n";
  return extension;
}

#endif // FILE_PATH_OPERATOR_H_
