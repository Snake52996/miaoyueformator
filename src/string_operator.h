#ifndef STRING_OPERATOR_H_
#define STRING_OPERATOR_H_

#include <iostream>
#include <cstring>
using namespace std;

void RemovePrefixAndSuffixSpace(string& str){
  string temp = "";
  int end_of_string = -1;
  bool started = false;
  for(int i = str.size() - 1; i >= 0; --i) if(str[i] != ' '){
  	end_of_string = i;
  	break;
  }
  for(int i = 0; i <= end_of_string; ++i){
  	if(str[i] != ' ') started = true;
  	if(started) temp.push_back(str[i]);
  }
  str = temp;
  return;
}

#endif //STRING_OPERATOR_H_
