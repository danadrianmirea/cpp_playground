#include <cstdint>
#include <iostream>
#include "bitwise.h"


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

