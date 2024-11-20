#include <cstdint>
#include <iostream>
#include "bitwise.h"


using namespace std;

void (*funcPtr)(int);

void realFunction(int n)
{
  std::cout << "n is: " << n << "\n";
}

inline void Print(int num)
{
  cout << "num is: " << num << "\n";
}

int32_t main()
{
  int num = 8;
  int &ref = num;
  int const &cRef = num;
  const int &cRef2 = num;
  const int const &cRef3 = num;
  const const const int gigaConst = 8; // valid but same as const int
  const const const int const const superGigaConst = 111; // valid but same as const int 

  funcPtr = realFunction;
  funcPtr(103);
  
  int *intPtr = new int;
  int* const constantPointer = new int;
  
  *intPtr = num;
  *constantPointer = num;
  *constantPointer = 13;
  Print(num);

  ref = 17;
  Print(num);

  Print(cRef3);

  //cRef = 88; // not valid, this reference is const
  //cRef2 = 99; // same as above, just defined differently: const int and int const are the same


  delete intPtr;
  intPtr = nullptr;
  delete constantPointer;
  //ptrToConstInt = nullptr;  // error, cannot assign a value to a constant pointer
  

  return 0;
}

