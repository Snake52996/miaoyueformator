/*
定义了绘图操作
*/
#ifndef SWEET_PRINTER_H_
#define SWEET_PRINTER_H_
#include<iostream>
#include<cstdio>
#include<windows.h>
#include<wincon.h>
#include<wchar.h>
#include<cstdlib>
using namespace std;

//减速显示函数,每个字符输出后停顿waits_after_each_word毫秒,结尾不换行
void PrintStringSlower(const char* str, const int& waits_after_each_word){
  for(int i=0; str[i]!='\0';++i){
    cout<<str[i];
    Sleep(waits_after_each_word);
  }
}

//减速显示函数,每个字符输出后停顿waits_after_each_word毫秒,结尾换行
void PrintStringAndEndlineSlower(const char* str, const int& waits_after_each_word){
  for(int i=0; str[i]!='\0';++i)
  {
    cout<<str[i];
    Sleep(waits_after_each_word);
  }
  cout<<endl;
}

//清空界面
inline void ClearCli(void){ system("cls"); }

#endif // SWEET_PRINTER_H_
