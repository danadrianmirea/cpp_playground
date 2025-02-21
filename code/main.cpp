#include <cstdint>
#include <iostream>

using namespace std;

template <typename T>
std::enable_if<std::is_integral_v<T>> PrintInteger(T)
{
  std::cout << "Integer value: " << T << "\n";
}

int32_t main()
{
  PrintInteger(3);
  return 0;
}


