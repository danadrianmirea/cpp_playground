#include <iostream>
#include "constAndPointers.h"

void (*funcPtr)(int);

void realFunction(int n)
{
  std::cout << "n is: " << n << "\n";
}

inline void Print(int num)
{
  std::cout << "num is: " << num << "\n";
}

void ConstPointerTestFunction()
{
  int num = 8;
  int num2 = 11;
  const int cNum = 13;
  int& ref = num;
  int const& cRef = num;
  const int& cRef2 = num;
  const int const& cRef3 = num;
  const const const int gigaConst = 8; // valid but same as const int (actually seems to depend on compiler and settings)
  const const const int const const superGigaConst = 111; // valid but same as const int 

  int* intPtr = new int;
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
  //constantPointerToConst = &num2; // invalid, cannot change a constant pointer value
  //*constantPointerToConst = 8888; // invalid, cannot change the value the pointer is pointing to because it's a pointer TO a const
  // essentially constantPointerToConst is just set once to some memory and can only read it's value
  //cRef = 88; // not valid, this reference is const
  //cRef2 = 99; // same as above, just defined differently: const int and int const are the same

  delete intPtr;
  intPtr = nullptr;
};