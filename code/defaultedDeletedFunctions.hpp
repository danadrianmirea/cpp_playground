#ifndef DEFAULTED_DELETED_FUNCTIONS_HPP
#define DEFAULTED_DELETED_FUNCTIONS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace defaulted_deleted_examples {

    // Class with defaulted functions
    class DefaultedClass {
    private:
        int data;
        std::string name;

    public:
        // Default constructor - explicitly defaulted
        DefaultedClass() = default;
        
        // Parameterized constructor
        DefaultedClass(int d, const std::string& n) : data(d), name(n) {}
        
        // Copy constructor - explicitly defaulted
        DefaultedClass(const DefaultedClass&) = default;
        
        // Copy assignment operator - explicitly defaulted
        DefaultedClass& operator=(const DefaultedClass&) = default;
        
        // Move constructor - explicitly defaulted
        DefaultedClass(DefaultedClass&&) = default;
        
        // Move assignment operator - explicitly defaulted
        DefaultedClass& operator=(DefaultedClass&&) = default;
        
        // Destructor - explicitly defaulted
        ~DefaultedClass() = default;
        
        // Member functions
        int get_data() const { return data; }
        const std::string& get_name() const { return name; }
        void set_data(int d) { data = d; }
        void set_name(const std::string& n) { name = n; }
    };

    // Class with deleted functions
    class DeletedClass {
    private:
        int data;
        std::string name;

    public:
        // Default constructor
        DeletedClass() : data(0), name("default") {}
        
        // Parameterized constructor
        DeletedClass(int d, const std::string& n) : data(d), name(n) {}
        
        // Copy constructor - deleted (non-copyable)
        DeletedClass(const DeletedClass&) = delete;
        
        // Copy assignment operator - deleted (non-copyable)
        DeletedClass& operator=(const DeletedClass&) = delete;
        
        // Move constructor - explicitly defaulted
        DeletedClass(DeletedClass&&) = default;
        
        // Move assignment operator - explicitly defaulted
        DeletedClass& operator=(DeletedClass&&) = default;
        
        // Destructor
        ~DeletedClass() = default;
        
        // Member functions
        int get_data() const { return data; }
        const std::string& get_name() const { return name; }
    };

    // Class with selective function deletion
    class SelectiveClass {
    private:
        int data;
        std::string name;

    public:
        // Default constructor
        SelectiveClass() : data(0), name("default") {}
        
        // Parameterized constructor
        SelectiveClass(int d, const std::string& n) : data(d), name(n) {}
        
        // Copy constructor - defaulted
        SelectiveClass(const SelectiveClass&) = default;
        
        // Copy assignment operator - defaulted
        SelectiveClass& operator=(const SelectiveClass&) = default;
        
        // Move constructor - deleted (non-movable)
        SelectiveClass(SelectiveClass&&) = delete;
        
        // Move assignment operator - deleted (non-movable)
        SelectiveClass& operator=(SelectiveClass&&) = delete;
        
        // Destructor
        ~SelectiveClass() = default;
        
        // Member functions
        int get_data() const { return data; }
        const std::string& get_name() const { return name; }
    };

    // Class with deleted specific overloads
    class OverloadClass {
    private:
        int data;

    public:
        // Default constructor
        OverloadClass() : data(0) {}
        
        // Parameterized constructor
        OverloadClass(int d) : data(d) {}
        
        // Copy constructor - defaulted
        OverloadClass(const OverloadClass&) = default;
        
        // Copy assignment operator - defaulted
        OverloadClass& operator=(const OverloadClass&) = default;
        
        // Destructor
        ~OverloadClass() = default;
        
        // Member function with deleted overloads
        void process(int value) {
            std::cout << "Processing int: " << value << std::endl;
        }
        
        // Deleted overload for double (prevents implicit conversion)
        void process(double) = delete;
        
        // Deleted overload for bool (prevents implicit conversion)
        void process(bool) = delete;
        
        int get_data() const { return data; }
    };

    // Class demonstrating RAII with deleted copy
    class RAIIClass {
    private:
        std::unique_ptr<int> resource;

    public:
        // Constructor
        RAIIClass(int value) : resource(std::make_unique<int>(value)) {
            std::cout << "RAIIClass constructed with value: " << value << std::endl;
        }
        
        // Copy constructor - deleted (unique_ptr is non-copyable)
        RAIIClass(const RAIIClass&) = delete;
        
        // Copy assignment operator - deleted
        RAIIClass& operator=(const RAIIClass&) = delete;
        
        // Move constructor - defaulted
        RAIIClass(RAIIClass&&) = default;
        
        // Move assignment operator - defaulted
        RAIIClass& operator=(RAIIClass&&) = default;
        
        // Destructor
        ~RAIIClass() {
            if (resource) {
                std::cout << "RAIIClass destroyed with value: " << *resource << std::endl;
            }
        }
        
        int get_value() const { return resource ? *resource : 0; }
    };

    // Function demonstrating deleted function calls
    void demonstrate_deleted_calls() {
        std::cout << "\n--- Demonstrating Deleted Function Calls ---" << std::endl;
        
        // This would cause compilation errors if uncommented:
        /*
        DeletedClass obj1(10, "test");
        DeletedClass obj2 = obj1;  // Error: copy constructor deleted
        DeletedClass obj3;
        obj3 = obj1;  // Error: copy assignment deleted
        */
        
        std::cout << "Deleted function calls would cause compilation errors." << std::endl;
    }

    // Demo function
    void demoDefaultedDeletedFunctions() {
        std::cout << "=== C++11 Defaulted and Deleted Functions Demo ===" << std::endl;

        // Defaulted functions demo
        std::cout << "\n--- Defaulted Functions Demo ---" << std::endl;
        
        DefaultedClass def1(42, "defaulted");
        DefaultedClass def2 = def1;  // Copy constructor
        DefaultedClass def3;
        def3 = def1;  // Copy assignment
        
        std::cout << "def1: " << def1.get_data() << ", " << def1.get_name() << std::endl;
        std::cout << "def2: " << def2.get_data() << ", " << def2.get_name() << std::endl;
        std::cout << "def3: " << def3.get_data() << ", " << def3.get_name() << std::endl;

        // Deleted functions demo
        std::cout << "\n--- Deleted Functions Demo ---" << std::endl;
        
        DeletedClass del1(100, "deleted");
        DeletedClass del2 = std::move(del1);  // Move constructor works
        
        std::cout << "del1: " << del1.get_data() << ", " << del1.get_name() << std::endl;
        std::cout << "del2: " << del2.get_data() << ", " << del2.get_name() << std::endl;

        // Selective deletion demo
        std::cout << "\n--- Selective Deletion Demo ---" << std::endl;
        
        SelectiveClass sel1(200, "selective");
        SelectiveClass sel2 = sel1;  // Copy constructor works
        
        std::cout << "sel1: " << sel1.get_data() << ", " << sel1.get_name() << std::endl;
        std::cout << "sel2: " << sel2.get_data() << ", " << sel2.get_name() << std::endl;

        // Overload deletion demo
        std::cout << "\n--- Overload Deletion Demo ---" << std::endl;
        
        OverloadClass ovl1(300);
        ovl1.process(42);  // Works
        
        // These would cause compilation errors if uncommented:
        /*
        ovl1.process(3.14);  // Error: double overload deleted
        ovl1.process(true);  // Error: bool overload deleted
        */
        
        std::cout << "ovl1: " << ovl1.get_data() << std::endl;
        std::cout << "Deleted overload calls would cause compilation errors." << std::endl;

        // RAII demo
        std::cout << "\n--- RAII Demo ---" << std::endl;
        
        RAIIClass raii1(500);
        RAIIClass raii2 = std::move(raii1);  // Move constructor works
        
        std::cout << "raii1: " << raii1.get_value() << std::endl;
        std::cout << "raii2: " << raii2.get_value() << std::endl;

        // Demonstrate deleted function calls
        demonstrate_deleted_calls();

        std::cout << "\n=== End of Defaulted and Deleted Functions Demo ===" << std::endl;
    }

} // namespace defaulted_deleted_examples

#endif // DEFAULTED_DELETED_FUNCTIONS_HPP 