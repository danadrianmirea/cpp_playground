#include <iostream>

void print(char* &&) { std::cout << "1" << std::endl; }
void print(char* &) { std::cout << "2" << std::endl; }

void testRValueLValue()
{
    char a = 'a';
    char* b = &a;
    print(&a);
    print(b);
}

int main() {
    testRValueLValue();
    return 0;
}