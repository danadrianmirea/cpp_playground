#ifndef SMART_POINTERS_HPP
#define SMART_POINTERS_HPP

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include <algorithm>
#include <chrono>
#include <thread>

namespace smart_pointer_examples {

    // Helper function for thread-safe printing
    void safe_print(const std::string& message) {
        std::cout << "[Thread " << std::this_thread::get_id() << "] " << message << std::endl;
    }

    // Example class for demonstrating smart pointers and move semantics
    class Resource {
    private:
        std::string name_;
        int* data_;
        size_t size_;

    public:
        // Constructor
        explicit Resource(const std::string& name, size_t size = 10) 
            : name_(name), size_(size) {
            data_ = new int[size_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = static_cast<int>(i);
            }
            std::cout << "Resource '" << name_ << "' constructed with " << size_ << " elements" << std::endl;
        }

        // Copy constructor
        Resource(const Resource& other) 
            : name_(other.name_ + "_copy"), size_(other.size_) {
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
            std::cout << "Resource '" << name_ << "' copy constructed" << std::endl;
        }

        // Copy assignment operator
        Resource& operator=(const Resource& other) {
            if (this != &other) {
                delete[] data_;
                name_ = other.name_ + "_assigned";
                size_ = other.size_;
                data_ = new int[size_];
                std::copy(other.data_, other.data_ + size_, data_);
                std::cout << "Resource '" << name_ << "' copy assigned" << std::endl;
            }
            return *this;
        }

        // Move constructor
        Resource(Resource&& other) noexcept 
            : name_(std::move(other.name_)), data_(other.data_), size_(other.size_) {
            other.data_ = nullptr;
            other.size_ = 0;
            std::cout << "Resource '" << name_ << "' move constructed" << std::endl;
        }

        // Move assignment operator
        Resource& operator=(Resource&& other) noexcept {
            if (this != &other) {
                delete[] data_;
                name_ = std::move(other.name_);
                data_ = other.data_;
                size_ = other.size_;
                other.data_ = nullptr;
                other.size_ = 0;
                std::cout << "Resource '" << name_ << "' move assigned" << std::endl;
            }
            return *this;
        }

        // Destructor
        ~Resource() {
            delete[] data_;
            std::cout << "Resource '" << name_ << "' destroyed" << std::endl;
        }

        // Member functions
        void print() const {
            std::cout << "Resource '" << name_ << "' data: ";
            for (size_t i = 0; i < size_; ++i) {
                std::cout << data_[i] << " ";
            }
            std::cout << std::endl;
        }

        const std::string& name() const { return name_; }
        size_t size() const { return size_; }
    };

    // Example 1: nullptr demonstration
    void demonstrate_nullptr() {
        std::cout << "\n--- nullptr Demonstration ---" << std::endl;
        
        // Traditional NULL vs nullptr
        int* ptr1 = nullptr;  // C++11 way
        int* ptr2 = NULL;     // Old C way
        
        std::cout << "ptr1 == nullptr: " << (ptr1 == nullptr) << std::endl;
        std::cout << "ptr2 == nullptr: " << (ptr2 == nullptr) << std::endl;
        
        // nullptr is strongly typed
        std::nullptr_t null_value = nullptr;
        std::cout << "null_value == nullptr: " << (null_value == nullptr) << std::endl;
        
        // Function overload resolution with nullptr
        auto overload_test = [](int* ptr) { std::cout << "Called with int*" << std::endl; };
        auto overload_test2 = [](std::nullptr_t ptr) { std::cout << "Called with nullptr_t" << std::endl; };
        
        overload_test(nullptr);  // Calls int* version
        overload_test2(nullptr); // Calls nullptr_t version
        
        // nullptr in containers
        std::vector<int*> ptr_vector = {nullptr, nullptr, nullptr};
        std::cout << "Vector with nullptr elements size: " << ptr_vector.size() << std::endl;
        
        // nullptr in comparisons
        int* test_ptr = nullptr;
        if (test_ptr == nullptr) {
            std::cout << "Pointer is null" << std::endl;
        }
        
        // nullptr in function parameters
        auto check_null = [](int* ptr) -> bool {
            return ptr == nullptr;
        };
        
        std::cout << "check_null(nullptr): " << check_null(nullptr) << std::endl;
    }

    // Example 2: std::unique_ptr demonstration
    void demonstrate_unique_ptr() {
        std::cout << "\n--- std::unique_ptr Demonstration ---" << std::endl;
        
        // Creating unique_ptr
        std::unique_ptr<Resource> ptr1(new Resource("Unique1", 5));
        std::unique_ptr<Resource> ptr2 = std::make_unique<Resource>("Unique2", 3);
        
        // Accessing members
        ptr1->print();
        ptr2->print();
        
        // Checking if pointer is valid
        std::cout << "ptr1 valid: " << (ptr1 != nullptr) << std::endl;
        std::cout << "ptr2 valid: " << (ptr2 != nullptr) << std::endl;
        
        // Moving unique_ptr (transfer ownership)
        std::unique_ptr<Resource> ptr3 = std::move(ptr1);
        std::cout << "After move:" << std::endl;
        std::cout << "ptr1 valid: " << (ptr1 != nullptr) << std::endl;
        std::cout << "ptr3 valid: " << (ptr3 != nullptr) << std::endl;
        
        if (ptr3) {
            ptr3->print();
        }
        
        // unique_ptr in containers
        std::vector<std::unique_ptr<Resource>> resource_vector;
        resource_vector.push_back(std::make_unique<Resource>("Vector1", 2));
        resource_vector.push_back(std::make_unique<Resource>("Vector2", 4));
        
        std::cout << "Vector size: " << resource_vector.size() << std::endl;
        for (const auto& ptr : resource_vector) {
            ptr->print();
        }
        
        // unique_ptr with custom deleter
        auto custom_deleter = [](Resource* r) {
            std::cout << "Custom deleter called for " << r->name() << std::endl;
            delete r;
        };
        
        std::unique_ptr<Resource, decltype(custom_deleter)> custom_ptr(
            new Resource("Custom", 3), custom_deleter);
        
        // unique_ptr with arrays
        std::unique_ptr<int[]> array_ptr(new int[5]{1, 2, 3, 4, 5});
        std::cout << "Array elements: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << array_ptr[i] << " ";
        }
        std::cout << std::endl;
        
        // Releasing ownership
        Resource* raw_ptr = ptr3.release();
        std::cout << "ptr3 valid after release: " << (ptr3 != nullptr) << std::endl;
        delete raw_ptr;  // Manual cleanup required after release
        
        // Resetting unique_ptr
        ptr2.reset(new Resource("Reset", 6));
        ptr2->print();
    }

    // Example 3: std::shared_ptr demonstration
    void demonstrate_shared_ptr() {
        std::cout << "\n--- std::shared_ptr Demonstration ---" << std::endl;
        
        // Creating shared_ptr
        std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>("Shared1", 4);
        std::shared_ptr<Resource> ptr2 = ptr1;  // Copy constructor
        
        std::cout << "ptr1 use count: " << ptr1.use_count() << std::endl;
        std::cout << "ptr2 use count: " << ptr2.use_count() << std::endl;
        
        // Multiple shared_ptr pointing to same object
        {
            std::shared_ptr<Resource> ptr3 = ptr1;
            std::shared_ptr<Resource> ptr4 = ptr1;
            
            std::cout << "Inside block - ptr1 use count: " << ptr1.use_count() << std::endl;
            ptr3->print();
        }  // ptr3 and ptr4 go out of scope
        
        std::cout << "Outside block - ptr1 use count: " << ptr1.use_count() << std::endl;
        
        // shared_ptr in containers
        std::vector<std::shared_ptr<Resource>> shared_vector;
        shared_vector.push_back(std::make_shared<Resource>("VectorShared1", 3));
        shared_vector.push_back(std::make_shared<Resource>("VectorShared2", 5));
        
        // Sharing ownership
        std::shared_ptr<Resource> shared1 = shared_vector[0];
        std::shared_ptr<Resource> shared2 = shared_vector[0];
        
        std::cout << "shared1 use count: " << shared1.use_count() << std::endl;
        
        // shared_ptr with custom deleter
        auto shared_deleter = [](Resource* r) {
            std::cout << "Shared custom deleter for " << r->name() << std::endl;
            delete r;
        };
        
        std::shared_ptr<Resource> custom_shared(
            new Resource("CustomShared", 4), shared_deleter);
        
        // shared_ptr aliasing constructor (demonstrating concept)
        // Note: This is a conceptual example - in practice, you'd alias to actual data members
        std::cout << "Aliasing constructor concept demonstrated" << std::endl;
        
        // shared_ptr weak_ptr interaction
        std::weak_ptr<Resource> weak_ptr = ptr1;
        std::cout << "weak_ptr expired: " << weak_ptr.expired() << std::endl;
        
        if (auto locked = weak_ptr.lock()) {
            std::cout << "Successfully locked weak_ptr" << std::endl;
            locked->print();
        }
    }

    // Example 4: std::weak_ptr demonstration
    void demonstrate_weak_ptr() {
        std::cout << "\n--- std::weak_ptr Demonstration ---" << std::endl;
        
        // Creating weak_ptr from shared_ptr
        std::shared_ptr<Resource> shared = std::make_shared<Resource>("WeakTest", 3);
        std::weak_ptr<Resource> weak = shared;
        
        std::cout << "shared use count: " << shared.use_count() << std::endl;
        std::cout << "weak expired: " << weak.expired() << std::endl;
        
        // Locking weak_ptr
        if (auto locked = weak.lock()) {
            std::cout << "Successfully locked weak_ptr" << std::endl;
            locked->print();
            std::cout << "shared use count after lock: " << shared.use_count() << std::endl;
        }
        
        // weak_ptr becomes expired
        shared.reset();
        std::cout << "After shared.reset():" << std::endl;
        std::cout << "weak expired: " << weak.expired() << std::endl;
        
        if (auto locked = weak.lock()) {
            std::cout << "This should not print" << std::endl;
        } else {
            std::cout << "weak_ptr is expired, cannot lock" << std::endl;
        }
        
        // weak_ptr in circular reference prevention
        struct Node {
            std::string name;
            std::shared_ptr<Node> next;
            std::weak_ptr<Node> prev;  // Prevents circular reference
            
            Node(const std::string& n) : name(n) {}
            
            ~Node() {
                std::cout << "Node '" << name << "' destroyed" << std::endl;
            }
        };
        
        auto node1 = std::make_shared<Node>("Node1");
        auto node2 = std::make_shared<Node>("Node2");
        
        node1->next = node2;
        node2->prev = node1;
        
        std::cout << "node1 use count: " << node1.use_count() << std::endl;
        std::cout << "node2 use count: " << node2.use_count() << std::endl;
        
        // Nodes will be properly destroyed due to weak_ptr
    }

    // Example 5: Move semantics demonstration
    void demonstrate_move_semantics() {
        std::cout << "\n--- Move Semantics Demonstration ---" << std::endl;
        
        // Creating resources
        Resource r1("Original", 4);
        Resource r2("Target", 2);
        
        std::cout << "Before move:" << std::endl;
        r1.print();
        r2.print();
        
        // Move assignment
        r2 = std::move(r1);
        
        std::cout << "After move assignment:" << std::endl;
        r2.print();
        
        // Move constructor
        Resource r3(std::move(r2));
        
        std::cout << "After move constructor:" << std::endl;
        r3.print();
        
        // std::move with containers
        std::vector<Resource> vec1;
        vec1.emplace_back("Vector1", 3);
        vec1.emplace_back("Vector2", 5);
        
        std::vector<Resource> vec2 = std::move(vec1);
        
        std::cout << "vec1 size after move: " << vec1.size() << std::endl;
        std::cout << "vec2 size after move: " << vec2.size() << std::endl;
        
        // std::forward for perfect forwarding
        auto forward_wrapper = [](auto&& arg) {
            return std::forward<decltype(arg)>(arg);
        };
        
        Resource temp("ForwardTest", 3);
        auto forwarded = forward_wrapper(std::move(temp));
        forwarded.print();
        
        // Move semantics with smart pointers
        std::unique_ptr<Resource> unique1 = std::make_unique<Resource>("UniqueMove", 4);
        std::unique_ptr<Resource> unique2 = std::move(unique1);
        
        std::cout << "unique1 valid after move: " << (unique1 != nullptr) << std::endl;
        std::cout << "unique2 valid after move: " << (unique2 != nullptr) << std::endl;
        
        if (unique2) {
            unique2->print();
        }
    }

    // Example 6: Smart pointers with move semantics
    void demonstrate_smart_pointers_with_move() {
        std::cout << "\n--- Smart Pointers with Move Semantics ---" << std::endl;
        
        // unique_ptr with move semantics
        auto create_unique = []() -> std::unique_ptr<Resource> {
            return std::make_unique<Resource>("FactoryCreated", 6);
        };
        
        std::unique_ptr<Resource> factory_ptr = create_unique();
        factory_ptr->print();
        
        // Moving unique_ptr between containers
        std::vector<std::unique_ptr<Resource>> source_vec;
        source_vec.push_back(std::make_unique<Resource>("Source1", 2));
        source_vec.push_back(std::make_unique<Resource>("Source2", 3));
        
        std::vector<std::unique_ptr<Resource>> dest_vec;
        
        // Move elements from source to destination
        for (auto& ptr : source_vec) {
            dest_vec.push_back(std::move(ptr));
        }
        
        std::cout << "Source vector size after move: " << source_vec.size() << std::endl;
        std::cout << "Destination vector size after move: " << dest_vec.size() << std::endl;
        
        // shared_ptr with move semantics
        std::shared_ptr<Resource> shared1 = std::make_shared<Resource>("SharedMove1", 4);
        std::shared_ptr<Resource> shared2 = std::move(shared1);
        
        std::cout << "shared1 use count after move: " << shared1.use_count() << std::endl;
        std::cout << "shared2 use count after move: " << shared2.use_count() << std::endl;
        
        // Moving shared_ptr to unique_ptr (if possible)
        auto shared_to_unique = [](std::shared_ptr<Resource> shared) -> std::unique_ptr<Resource> {
            if (shared.use_count() == 1) {
                return std::unique_ptr<Resource>(shared.get());
            }
            return nullptr;
        };
        
        auto unique_from_shared = shared_to_unique(shared2);
        if (unique_from_shared) {
            std::cout << "Successfully converted shared_ptr to unique_ptr" << std::endl;
        }
    }

    // Example 7: nullptr with smart pointers
    void demonstrate_nullptr_with_smart_pointers() {
        std::cout << "\n--- nullptr with Smart Pointers ---" << std::endl;
        
        // unique_ptr with nullptr
        std::unique_ptr<Resource> unique_null = nullptr;
        std::cout << "unique_null valid: " << (unique_null != nullptr) << std::endl;
        
        // shared_ptr with nullptr
        std::shared_ptr<Resource> shared_null = nullptr;
        std::cout << "shared_null use count: " << shared_null.use_count() << std::endl;
        
        // weak_ptr with nullptr
        std::weak_ptr<Resource> weak_null = shared_null;
        std::cout << "weak_null expired: " << weak_null.expired() << std::endl;
        
        // Checking for null before use
        auto safe_print = [](const std::unique_ptr<Resource>& ptr) {
            if (ptr != nullptr) {
                ptr->print();
            } else {
                std::cout << "Pointer is null" << std::endl;
            }
        };
        
        safe_print(unique_null);
        
        // Resetting to nullptr
        auto temp_ptr = std::make_unique<Resource>("Temp", 3);
        temp_ptr.reset(nullptr);
        std::cout << "temp_ptr valid after reset: " << (temp_ptr != nullptr) << std::endl;
        
        // nullptr in comparisons
        std::unique_ptr<Resource> test_ptr = nullptr;
        if (test_ptr == nullptr) {
            std::cout << "test_ptr is null" << std::endl;
        }
        
        // nullptr in function parameters
        auto process_ptr = [](std::unique_ptr<Resource> ptr) {
            if (ptr == nullptr) {
                std::cout << "Received null pointer" << std::endl;
            } else {
                std::cout << "Processing resource: " << ptr->name() << std::endl;
            }
        };
        
        process_ptr(nullptr);
        process_ptr(std::make_unique<Resource>("ProcessTest", 2));
    }

    // Example 8: Advanced smart pointer patterns
    void demonstrate_advanced_patterns() {
        std::cout << "\n--- Advanced Smart Pointer Patterns ---" << std::endl;
        
        // PIMPL idiom with unique_ptr
        class PimplExample {
        private:
            class Impl;
            std::unique_ptr<Impl> pImpl;
            
        public:
            PimplExample();
            ~PimplExample();
            void doSomething();
        };
        
        // RAII with smart pointers
        class RAIIExample {
        private:
            std::unique_ptr<Resource> resource_;
            
        public:
            RAIIExample() : resource_(std::make_unique<Resource>("RAII", 5)) {
                std::cout << "RAIIExample constructed" << std::endl;
            }
            
            ~RAIIExample() {
                std::cout << "RAIIExample destroyed" << std::endl;
            }
            
            void useResource() {
                if (resource_) {
                    resource_->print();
                }
            }
        };
        
        RAIIExample raii;
        raii.useResource();
        
        // Factory pattern with smart pointers
        auto createResource = [](const std::string& name, size_t size) -> std::unique_ptr<Resource> {
            if (size == 0) {
                return nullptr;
            }
            return std::make_unique<Resource>(name, size);
        };
        
        auto resource1 = createResource("Factory1", 3);
        auto resource2 = createResource("Factory2", 0);  // Returns nullptr
        
        if (resource1) resource1->print();
        if (resource2) resource2->print();
        else std::cout << "resource2 is null" << std::endl;
        
        // Observer pattern with weak_ptr
        class Observer {
        public:
            virtual void update(const std::string& message) = 0;
            virtual ~Observer() = default;
        };
        
        class Subject {
        private:
            std::vector<std::weak_ptr<Observer>> observers_;
            
        public:
            void addObserver(std::weak_ptr<Observer> observer) {
                observers_.push_back(observer);
            }
            
            void notify(const std::string& message) {
                observers_.erase(
                    std::remove_if(observers_.begin(), observers_.end(),
                        [&message](const std::weak_ptr<Observer>& weak_obs) {
                            if (auto obs = weak_obs.lock()) {
                                obs->update(message);
                                return false;
                            }
                            return true;  // Remove expired observers
                        }),
                    observers_.end()
                );
            }
        };
        
        // Smart pointer arrays
        std::unique_ptr<Resource[]> resource_array(new Resource[3]{
            Resource("Array1", 2),
            Resource("Array2", 3),
            Resource("Array3", 4)
        });
        
        for (int i = 0; i < 3; ++i) {
            resource_array[i].print();
        }
    }

    // Demo function
    void demoSmartPointers() {
        std::cout << "=== C++11 Smart Pointers, Move Semantics, and nullptr Demo ===" << std::endl;

        demonstrate_nullptr();
        demonstrate_unique_ptr();
        demonstrate_shared_ptr();
        demonstrate_weak_ptr();
        demonstrate_move_semantics();
        demonstrate_smart_pointers_with_move();
        demonstrate_nullptr_with_smart_pointers();
        demonstrate_advanced_patterns();

        std::cout << "\n=== End of Smart Pointers, Move Semantics, and nullptr Demo ===" << std::endl;
    }

} // namespace smart_pointer_examples

#endif // SMART_POINTERS_HPP 