#pragma once
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class Debug {

public:

  template<typename T, typename... Args>
  static void Log(const T& s, const Args&... args) {
    cout << s;
    if constexpr(sizeof...(args) > 0) Log(args...);
	  else  cout << endl;
  }

};
