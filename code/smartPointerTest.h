#pragma once

#include <cstdint>
#include <iostream>

using namespace std;

class ExampleClass
{
public:
  ExampleClass() : val(0) { std::cout << "ExampleClass ct\n"; };
  ~ExampleClass() { std::cout << "ExampleClass dt\n"; }
  void SetVal(const int n) { val = n; }
  int GetVal() { return val; }
private:
  int val;
};

void TestSmartPointer()
{
  std::unique_ptr<ExampleClass> g(new ExampleClass());
  g->SetVal(7);
  std::cout << g->GetVal()  << "\n";
} 