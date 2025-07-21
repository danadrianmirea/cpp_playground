// variableTemplates.hpp

#ifndef VARIABLE_TEMPLATES_HPP
#define VARIABLE_TEMPLATES_HPP

#include <iostream>

// Variable template example
template<typename T>
constexpr T pi = T(3.1415926535897932385);

// Function to demonstrate variable templates
inline void demoVariableTemplates() {
    std::cout << "Value of pi as float: " << pi<float> << std::endl;
    std::cout << "Value of pi as double: " << pi<double> << std::endl;
    std::cout << "Value of pi as long double: " << pi<long double> << std::endl;
}

#endif // VARIABLE_TEMPLATES_HPP