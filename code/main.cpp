#include <cstdint>
#include <iostream>
#include "example.h"
#include "datastructures.h"
#include "testclass.h"
#include "log.h"
#include "tokenizer.h"
#include "staticmember.h"
#include "generictest.hpp"
#include "bitwise.h"
#include "interrupt_example.h"

using namespace std;

union kkt
{
  int intlike;
  float floatlike;
};

int32_t main()
{
  kkt k1;
  k1.intlike = 8;
  std::cout << "as int: " << k1.intlike << endl;
  std::cout << "as float: " << k1.floatlike << endl;
  return 0;
}