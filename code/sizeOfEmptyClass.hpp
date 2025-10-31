#pragma once

#include <iostream>

// Empty class - no data members, no virtual functions
class EmptyClass {
    // Nothing inside
};

// Another empty class
class AnotherEmptyClass {
    // Also empty
};

// Class with a single byte member
class SingleByte {
    char c;
};

// Class deriving from empty class
class DerivedFromEmpty : public EmptyClass {
    int value;
};

// Class with empty class as member
class HasEmptyMember {
public:
    EmptyClass empty;
    int value;
};

// Multiple inheritance from empty classes
class MultipleEmpty : public EmptyClass, public AnotherEmptyClass {
    int value;
};

// Empty class with virtual function (has vtable pointer, so size > 1)
class EmptyWithVirtual {
public:
    virtual ~EmptyWithVirtual() {}
};

// Function to demonstrate size of empty classes
inline void sizeOfEmptyClassDemo() {
    std::cout << "=== Size of Empty Classes Demonstration ===\n\n";
    
    // 1. Basic empty class size
    std::cout << "1. Basic Empty Class:\n";
    std::cout << "   sizeof(EmptyClass) = " << sizeof(EmptyClass) << " byte(s)\n";
    std::cout << "   Note: Size is 1, not 0!\n";
    std::cout << "   Reason: Ensures each object has a unique address in arrays.\n\n";
    
    // 2. Why size is 1, not 0
    std::cout << "2. Why Empty Classes Have Size 1:\n";
    EmptyClass arr[10];
    std::cout << "   EmptyClass arr[10];\n";
    std::cout << "   &arr[0] = " << static_cast<const void*>(&arr[0]) << "\n";
    std::cout << "   &arr[1] = " << static_cast<const void*>(&arr[1]) << "\n";
    std::cout << "   Difference: " << reinterpret_cast<const char*>(&arr[1]) - reinterpret_cast<const char*>(&arr[0]) 
              << " byte(s)\n";
    std::cout << "   If size were 0, all elements would have the same address!\n\n";
    
    // 3. Comparison with other types
    std::cout << "3. Size Comparison:\n";
    std::cout << "   sizeof(EmptyClass)       = " << sizeof(EmptyClass) << " byte(s)\n";
    std::cout << "   sizeof(char)            = " << sizeof(char) << " byte(s)\n";
    std::cout << "   sizeof(bool)            = " << sizeof(bool) << " byte(s)\n";
    std::cout << "   sizeof(SingleByte)      = " << sizeof(SingleByte) << " byte(s)\n";
    std::cout << "   sizeof(int)             = " << sizeof(int) << " byte(s)\n";
    std::cout << "   sizeof(void*)           = " << sizeof(void*) << " byte(s)\n\n";
    
    // 4. Inheritance scenarios
    std::cout << "4. Inheritance Scenarios:\n";
    std::cout << "   sizeof(DerivedFromEmpty) = " << sizeof(DerivedFromEmpty) << " byte(s)\n";
    std::cout << "   sizeof(EmptyClass) + sizeof(int) = " 
              << sizeof(EmptyClass) + sizeof(int) << " byte(s)\n";
    std::cout << "   Note: Empty base class optimization (EBO) may apply!\n";
    std::cout << "   The empty base doesn't always add to the size.\n\n";
    
    // 5. Empty class as member
    std::cout << "5. Empty Class as Member:\n";
    std::cout << "   sizeof(HasEmptyMember)  = " << sizeof(HasEmptyMember) << " byte(s)\n";
    std::cout << "   Expected: sizeof(EmptyClass) + sizeof(int) + padding = "
              << sizeof(EmptyClass) + sizeof(int) << "+ padding\n";
    HasEmptyMember hasEmpty;
    std::cout << "   Address of empty member: " << static_cast<const void*>(&hasEmpty.empty) << "\n";
    std::cout << "   Address of value member: " << static_cast<const void*>(&hasEmpty.value) << "\n\n";
    
    // 6. Multiple inheritance from empty classes
    std::cout << "6. Multiple Inheritance from Empty Classes:\n";
    std::cout << "   sizeof(MultipleEmpty) = " << sizeof(MultipleEmpty) << " byte(s)\n";
    std::cout << "   Note: Compiler may optimize empty base classes!\n\n";
    
    // 7. Empty class with virtual function
    std::cout << "7. Empty Class with Virtual Function:\n";
    std::cout << "   sizeof(EmptyWithVirtual) = " << sizeof(EmptyWithVirtual) << " byte(s)\n";
    std::cout << "   Note: Virtual function adds vtable pointer (" << sizeof(void*) 
              << " bytes on this system).\n";
    std::cout << "   So size is " << sizeof(void*) << ", not 1!\n\n";
    
    // 8. Practical demonstration
    std::cout << "8. Practical Demonstration - Array Addressing:\n";
    EmptyClass emptyObjs[5];
    for (int i = 0; i < 5; ++i) {
        std::cout << "   emptyObjs[" << i << "] at address: " 
                  << static_cast<const void*>(&emptyObjs[i]) << "\n";
    }
    std::cout << "   All addresses are unique, separated by at least 1 byte.\n\n";
    
    // 9. Address uniqueness guarantee
    std::cout << "9. Address Uniqueness Guarantee:\n";
    EmptyClass e1, e2, e3;
    std::cout << "   &e1 = " << static_cast<const void*>(&e1) << "\n";
    std::cout << "   &e2 = " << static_cast<const void*>(&e2) << "\n";
    std::cout << "   &e3 = " << static_cast<const void*>(&e3) << "\n";
    std::cout << "   All objects must have distinct addresses (C++ requirement).\n\n";
    
    // 10. Summary
    std::cout << "10. Summary:\n";
    std::cout << "    - Empty classes have size 1 (not 0)\n";
    std::cout << "    - This ensures each object has a unique address\n";
    std::cout << "    - Required for arrays to work correctly\n";
    std::cout << "    - Empty Base Class Optimization (EBO) can eliminate this size in inheritance\n";
    std::cout << "    - Virtual functions add overhead (vtable pointer)\n";
    
    std::cout << "\n=== Demonstration Complete ===\n";
}
