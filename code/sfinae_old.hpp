#pragma once 

#include <iostream>
#include <type_traits>

// Enable this function only if T is an integral type
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type print(T value) 
{
  std::cout << "Integral: " << value << std::endl;
}

// Enable this function only if T is a floating-point type
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, void>::type
print(T value) 
{
  std::cout << "Floating point: " << value << std::endl;
}

int TestCPP11_Sfinae() 
{
  print(42);
  print(3.14);
  return 0;
}
