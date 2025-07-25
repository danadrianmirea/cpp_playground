#ifndef DELEGATING_CONSTRUCTORS_HPP
#define DELEGATING_CONSTRUCTORS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace delegating_constructor_examples {

    // Example 1: Simple delegating constructors
    class SimpleClass {
    private:
        int value;
        std::string name;
        bool initialized;

    public:
        // Primary constructor with all parameters
        SimpleClass(int val, const std::string& n, bool init) 
            : value(val), name(n), initialized(init) {
            std::cout << "Primary constructor called with: " << val << ", " << n << ", " << init << std::endl;
        }

        // Delegating constructor - calls primary constructor with default values
        SimpleClass(int val, const std::string& n) 
            : SimpleClass(val, n, true) {
            std::cout << "Delegating constructor (int, string) called" << std::endl;
        }

        // Delegating constructor - calls the previous delegating constructor
        SimpleClass(int val) 
            : SimpleClass(val, "default_name") {
            std::cout << "Delegating constructor (int) called" << std::endl;
        }

        // Default constructor - delegates to the int constructor
        SimpleClass() 
            : SimpleClass(0) {
            std::cout << "Default constructor called" << std::endl;
        }

        void print() const {
            std::cout << "SimpleClass: value=" << value << ", name='" << name 
                      << "', initialized=" << initialized << std::endl;
        }
    };

    // Example 2: Delegating constructors with different parameter types
    class ComplexClass {
    private:
        double x, y;
        std::string label;
        std::vector<int> data;

    public:
        // Primary constructor
        ComplexClass(double x_val, double y_val, const std::string& lbl, const std::vector<int>& d)
            : x(x_val), y(y_val), label(lbl), data(d) {
            std::cout << "Primary constructor called with: (" << x_val << ", " << y_val 
                      << "), '" << lbl << "', data size: " << d.size() << std::endl;
        }

        // Delegating constructor - creates empty vector
        ComplexClass(double x_val, double y_val, const std::string& lbl)
            : ComplexClass(x_val, y_val, lbl, std::vector<int>{}) {
            std::cout << "Delegating constructor (double, double, string) called" << std::endl;
        }

        // Delegating constructor - uses default label
        ComplexClass(double x_val, double y_val)
            : ComplexClass(x_val, y_val, "default_label") {
            std::cout << "Delegating constructor (double, double) called" << std::endl;
        }

        // Delegating constructor - single parameter
        ComplexClass(double val)
            : ComplexClass(val, val) {
            std::cout << "Delegating constructor (double) called" << std::endl;
        }

        // Default constructor
        ComplexClass()
            : ComplexClass(0.0) {
            std::cout << "Default constructor called" << std::endl;
        }

        void print() const {
            std::cout << "ComplexClass: (" << x << ", " << y << "), label='" << label 
                      << "', data size: " << data.size() << std::endl;
        }
    };

    // Example 3: Delegating constructors with member initialization
    class ResourceManager {
    private:
        std::string resource_name;
        int resource_id;
        bool is_allocated;
        std::unique_ptr<int[]> data;

    public:
        // Primary constructor
        ResourceManager(const std::string& name, int id, bool allocated, size_t size)
            : resource_name(name), resource_id(id), is_allocated(allocated) {
            if (allocated && size > 0) {
                data = std::make_unique<int[]>(size);
                std::cout << "Allocated " << size << " integers for resource" << std::endl;
            }
            std::cout << "Primary constructor called: " << name << ", id: " << id 
                      << ", allocated: " << allocated << std::endl;
        }

        // Delegating constructor - no allocation
        ResourceManager(const std::string& name, int id)
            : ResourceManager(name, id, false, 0) {
            std::cout << "Delegating constructor (string, int) called" << std::endl;
        }

        // Delegating constructor - with allocation
        ResourceManager(const std::string& name, int id, size_t size)
            : ResourceManager(name, id, true, size) {
            std::cout << "Delegating constructor (string, int, size_t) called" << std::endl;
        }

        // Default constructor
        ResourceManager()
            : ResourceManager("default_resource", 0) {
            std::cout << "Default constructor called" << std::endl;
        }

        void print() const {
            std::cout << "ResourceManager: name='" << resource_name << "', id=" << resource_id 
                      << ", allocated=" << is_allocated << ", data ptr: " << (data ? "valid" : "null") << std::endl;
        }
    };

    // Example 4: Delegating constructors with inheritance
    class BaseClass {
    protected:
        int base_value;
        std::string base_name;

    public:
        BaseClass(int val, const std::string& name) 
            : base_value(val), base_name(name) {
            std::cout << "BaseClass constructor: " << val << ", " << name << std::endl;
        }

        BaseClass(int val) 
            : BaseClass(val, "base_default") {
            std::cout << "BaseClass delegating constructor called" << std::endl;
        }

        virtual void print() const {
            std::cout << "BaseClass: value=" << base_value << ", name='" << base_name << "'" << std::endl;
        }
    };

    class DerivedClass : public BaseClass {
    private:
        double derived_value;

    public:
        // Primary constructor
        DerivedClass(int base_val, const std::string& base_name, double derived_val)
            : BaseClass(base_val, base_name), derived_value(derived_val) {
            std::cout << "DerivedClass primary constructor called" << std::endl;
        }

        // Delegating constructor - uses BaseClass delegating constructor
        DerivedClass(int base_val, double derived_val)
            : DerivedClass(base_val, "derived_default", derived_val) {
            std::cout << "DerivedClass delegating constructor called" << std::endl;
        }

        // Delegating constructor - single parameter
        DerivedClass(double derived_val)
            : DerivedClass(0, derived_val) {
            std::cout << "DerivedClass single parameter constructor called" << std::endl;
        }

        // Default constructor
        DerivedClass()
            : DerivedClass(0.0) {
            std::cout << "DerivedClass default constructor called" << std::endl;
        }

        void print() const override {
            BaseClass::print();
            std::cout << "DerivedClass: derived_value=" << derived_value << std::endl;
        }
    };

    // Example 5: Delegating constructors with template parameters
    template<typename T>
    class TemplateClass {
    private:
        T value;
        std::string description;
        bool is_valid;

    public:
        // Primary constructor
        TemplateClass(const T& val, const std::string& desc, bool valid)
            : value(val), description(desc), is_valid(valid) {
            std::cout << "TemplateClass primary constructor called" << std::endl;
        }

        // Delegating constructor
        TemplateClass(const T& val, const std::string& desc)
            : TemplateClass(val, desc, true) {
            std::cout << "TemplateClass delegating constructor called" << std::endl;
        }

        // Delegating constructor - single parameter
        TemplateClass(const T& val)
            : TemplateClass(val, "template_default") {
            std::cout << "TemplateClass single parameter constructor called" << std::endl;
        }

        // Default constructor
        TemplateClass()
            : TemplateClass(T{}) {
            std::cout << "TemplateClass default constructor called" << std::endl;
        }

        void print() const {
            std::cout << "TemplateClass: value=" << value << ", description='" << description 
                      << "', valid=" << is_valid << std::endl;
        }
    };

    // Example 6: Delegating constructors with move semantics
    class MoveableClass {
    private:
        std::vector<int> data;
        std::string name;
        int id;

    public:
        // Primary constructor
        MoveableClass(std::vector<int>&& vec, std::string&& str, int identifier)
            : data(std::move(vec)), name(std::move(str)), id(identifier) {
            std::cout << "MoveableClass primary constructor called" << std::endl;
        }

        // Delegating constructor - copies data
        MoveableClass(const std::vector<int>& vec, const std::string& str, int identifier)
            : MoveableClass(std::vector<int>(vec), std::string(str), identifier) {
            std::cout << "MoveableClass delegating constructor (copy) called" << std::endl;
        }

        // Delegating constructor - default id
        MoveableClass(const std::vector<int>& vec, const std::string& str)
            : MoveableClass(vec, str, 0) {
            std::cout << "MoveableClass delegating constructor (default id) called" << std::endl;
        }

        // Default constructor
        MoveableClass()
            : MoveableClass(std::vector<int>{}, std::string{}, 0) {
            std::cout << "MoveableClass default constructor called" << std::endl;
        }

        void print() const {
            std::cout << "MoveableClass: data size=" << data.size() << ", name='" << name 
                      << "', id=" << id << std::endl;
        }
    };

    // Demo function
    void demoDelegatingConstructors() {
        std::cout << "=== C++11 Delegating Constructors Demo ===" << std::endl;

        // Example 1: Simple delegating constructors
        std::cout << "\n--- Example 1: Simple Delegating Constructors ---" << std::endl;
        SimpleClass sc1(42, "test", false);
        SimpleClass sc2(42, "test");
        SimpleClass sc3(42);
        SimpleClass sc4;
        
        sc1.print();
        sc2.print();
        sc3.print();
        sc4.print();

        // Example 2: Complex delegating constructors
        std::cout << "\n--- Example 2: Complex Delegating Constructors ---" << std::endl;
        ComplexClass cc1(3.14, 2.71, "pi_e", std::vector<int>{1, 2, 3});
        ComplexClass cc2(3.14, 2.71, "pi_e");
        ComplexClass cc3(3.14, 2.71);
        ComplexClass cc4(3.14);
        ComplexClass cc5;
        
        cc1.print();
        cc2.print();
        cc3.print();
        cc4.print();
        cc5.print();

        // Example 3: Resource management
        std::cout << "\n--- Example 3: Resource Management ---" << std::endl;
        ResourceManager rm1("my_resource", 123, true, 10);
        ResourceManager rm2("my_resource", 123);
        ResourceManager rm3("my_resource", 123, 5);
        ResourceManager rm4;
        
        rm1.print();
        rm2.print();
        rm3.print();
        rm4.print();

        // Example 4: Inheritance
        std::cout << "\n--- Example 4: Inheritance ---" << std::endl;
        DerivedClass dc1(10, "derived_name", 3.14);
        DerivedClass dc2(10, 3.14);
        DerivedClass dc3(3.14);
        DerivedClass dc4;
        
        dc1.print();
        dc2.print();
        dc3.print();
        dc4.print();

        // Example 5: Template classes
        std::cout << "\n--- Example 5: Template Classes ---" << std::endl;
        TemplateClass<int> tc1(42, "int_value", true);
        TemplateClass<int> tc2(42, "int_value");
        TemplateClass<int> tc3(42);
        TemplateClass<int> tc4;
        
        tc1.print();
        tc2.print();
        tc3.print();
        tc4.print();

        // Example 6: Move semantics
        std::cout << "\n--- Example 6: Move Semantics ---" << std::endl;
        std::vector<int> vec1{1, 2, 3, 4, 5};
        std::string str1 = "test_string";
        
        MoveableClass mc1(std::move(vec1), std::move(str1), 42);
        MoveableClass mc2(std::vector<int>{6, 7, 8}, std::string{"another"}, 123);
        MoveableClass mc3;
        
        mc1.print();
        mc2.print();
        mc3.print();

        std::cout << "\n=== End of Delegating Constructors Demo ===" << std::endl;
    }

} // namespace delegating_constructor_examples

#endif // DELEGATING_CONSTRUCTORS_HPP 