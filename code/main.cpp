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
  int num2 = 11;
  const int cNum = 13;
  int &ref = num;
  int const &cRef = num;
  const int &cRef2 = num;
  const int const &cRef3 = num;
  const const const int gigaConst = 8; // valid but same as const int
  const const const int const const superGigaConst = 111; // valid but same as const int 

  int *intPtr = new int;
  int* const constantPointer = &num;
  int const* pointerToConst = &num;
  int const* const constantPointerToConst = &num;

  funcPtr = realFunction;
  funcPtr(103);
  
  *intPtr = num;
  *intPtr = 14;
  Print(num);

  ref = 17;
  Print(num);

  Print(cRef3);

  Print(gigaConst);
  Print(superGigaConst);

  int* changeConst = const_cast<int*>(&cNum);
  *changeConst = 33;
  Print(cNum);

  *constantPointer = num;
  *constantPointer = 13;
  //constantPointer = &num2; // invalid, this is a constant pointer
  Print(num);

  pointerToConst = &num;
  Print(*pointerToConst);
  pointerToConst = &num2;
  Print(*pointerToConst);
  //*pointerToConst = 7777; // invalid, this is a pointer to a constant int


  //cRef = 88; // not valid, this reference is const
  //cRef2 = 99; // same as above, just defined differently: const int and int const are the same

  delete intPtr;
  intPtr = nullptr;

  return 0;
}

