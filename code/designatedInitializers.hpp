 #pragma once
 #include <iostream>
 #include <string>
 
 // Simple aggregates
 struct PersonDI {
     std::string name;
     int age;
     double heightMeters;
 };
 
 struct PointDI {
     int x;
     int y;
 };
 
 struct RectDI {
     PointDI topLeft;
     PointDI bottomRight;
 };
 
 #if !defined(HAS_CXX_DESIGNATED_INITIALIZERS)
   #if defined(__clang__) || defined(__GNUC__) || defined(_MSC_VER)
     #define HAS_CXX_DESIGNATED_INITIALIZERS 1
   #else
     #define HAS_CXX_DESIGNATED_INITIALIZERS 0
   #endif
 #endif
 
 inline void demo_designatedInitializers() {
     std::cout << "--- Designated Initializers Demo ---\n";
 
 #if HAS_CXX_DESIGNATED_INITIALIZERS
     // Using designated initializers (supported by many compilers)
    // Keep declaration order for best portability
    PersonDI alice{ .name = "Alice", .age = 30, .heightMeters = 1.70 };
     RectDI r{ .topLeft = {.x = 0, .y = 0}, .bottomRight = {.x = 10, .y = 20} };
 #else
     // Fallback: regular aggregate initialization by order
     PersonDI alice{ "Alice", 30, 1.70 };
     RectDI r{ {0, 0}, {10, 20} };
 #endif
 
     std::cout << "PersonDI{name='" << alice.name
               << "', age=" << alice.age
               << ", height=" << alice.heightMeters << "}\n";
 
     std::cout << "RectDI{topLeft=(" << r.topLeft.x << ", " << r.topLeft.y
               << "), bottomRight=(" << r.bottomRight.x << ", " << r.bottomRight.y
               << ")}\n";
 }

