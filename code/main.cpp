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

void (*funcPtr)(int);

void realFunction(int n)
{
  std::cout << "n is: \n" << n;
}

int32_t main()
{
  int num = 8;
  int &ref = num;
  int const &cRef = num;

  funcPtr = realFunction;
  
  int *intPtr = new int;
  int* const ptrToConstInt = new int;
  
  *intPtr = num;
  *ptrToConstInt = num;
  *ptrToConstInt = 13;
  
  //cout << "b is: " << *c;
  delete 


  return 0;
}

