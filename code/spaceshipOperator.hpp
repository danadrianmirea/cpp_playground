 #pragma once
#include <algorithm>
#include <compare>
#include <iostream>
#include <vector>
 
 // Demonstrates C++20 three-way comparison (operator<=>)
 struct Version {
     int majorVersion {};
     int minorVersion {};
     int patchVersion {};
 
     // Defaulted three-way comparison compares members lexicographically
     // and also generates == automatically.
     auto operator<=>(const Version& other) const = default;
 };
 
 inline std::ostream& operator<<(std::ostream& outputStream, const Version& version) {
     outputStream << version.majorVersion << "." << version.minorVersion << "." << version.patchVersion;
     return outputStream;
 }
 
 inline void demo_spaceshipOperator() {
     std::cout << "--- Three-way Comparison (<=>) Demo ---\n";
     std::cout << std::boolalpha;
 
     Version versionA{1, 2, 0};
     Version versionB{1, 2, 1};
     Version versionC{1, 2, 0};
 
     // Equality is synthesized alongside <=> when defaulted
     std::cout << "versionA == versionB: " << (versionA == versionB) << "\n";
     std::cout << "versionA == versionC: " << (versionA == versionC) << "\n";
 
     // The result type here is std::strong_ordering because all members are ints
     auto compareAB = (versionA <=> versionB);
     if (compareAB < 0) {
         std::cout << "versionA < versionB\n";
     } else if (compareAB > 0) {
         std::cout << "versionA > versionB\n";
     } else {
         std::cout << "versionA == versionB\n";
     }
 
     // Sorting works out of the box with <=>
     std::vector<Version> versionList{
         {1, 4, 2}, {2, 0, 0}, {1, 2, 10}, {1, 2, 0}, {0, 9, 5}
     };
     std::ranges::sort(versionList);
 
     std::cout << "Sorted versions: ";
     for (const Version& v : versionList) {
         std::cout << v << ' ';
     }
     std::cout << "\n";
 }