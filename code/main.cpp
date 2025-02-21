#include <cstdint>
#include <iostream>

using namespace std;

template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type Print(T val)
{
  std::cout << "Integer value: " << val << "\n";
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, void>::type Print(T val)
{
  std::cout << "Float value: " << val << "\n";
}

int32_t main()
{
  Print(3);
  Print(5.3f);
  return 0;
}


