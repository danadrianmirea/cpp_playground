#pragma once

void const_init() {
    //const int a; // compilation error:  error: uninitialized 'const a'
    const int a = 3;
    std::cout << "a: " << a << std::endl;
}