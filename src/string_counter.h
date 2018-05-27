#ifndef STRING_COUNTER_H_
#define STRING_COUNTER_H_
#include<iostream>
#include<cstring>
using namespace std;

struct string_counter{
 public:
  string_counter(){
    counter_ = "";
  }
  void initialize(const int& size_of_counter){
    size_of_counter_ = size_of_counter;
    for(int i = 1; i <= size_of_counter_; ++i) counter_.push_back('0');
  }
  bool add_one(void){
    return inbuild_add_one(size_of_counter_ - 1);
  }
  const string& get_counter(void){
    return counter_;
  }
 private:
  bool inbuild_add_one(const int& i){
    if(counter_[i] < '9'){
      counter_[i] += 1;
      return true;
    }else{
      if(i == 0) return false;
      counter_[i] = '0';
      bool result = inbuild_add_one(i - 1);
      if(!result) counter_[i] = '9';
      return result;
    }
  }
  string counter_;
  int size_of_counter_;
};

#endif // STRING_COUNTER_H_
