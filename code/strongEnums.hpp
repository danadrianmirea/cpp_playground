#ifndef STRONG_ENUMS_HPP
#define STRONG_ENUMS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <type_traits>

namespace strong_enum_examples {

    // Basic strongly typed enum examples
    void basic_enum_class_examples() {
        std::cout << "=== Basic Enum Class Examples ===" << std::endl;
        
        // Simple enum class
        enum class Color { Red, Green, Blue };
        
        Color c1 = Color::Red;
        Color c2 = Color::Green;
        Color c3 = Color::Blue;
        
        std::cout << "Color 1: " << static_cast<int>(c1) << std::endl;
        std::cout << "Color 2: " << static_cast<int>(c2) << std::endl;
        std::cout << "Color 3: " << static_cast<int>(c3) << std::endl;
        
        // Enum class with explicit values
        enum class Direction { North = 0, South = 1, East = 2, West = 3 };
        
        Direction dir1 = Direction::North;
        Direction dir2 = Direction::South;
        
        std::cout << "Direction 1: " << static_cast<int>(dir1) << std::endl;
        std::cout << "Direction 2: " << static_cast<int>(dir2) << std::endl;
        
        // Enum class with custom underlying type
        enum class Status : uint8_t { 
            Pending = 0, 
            Active = 1, 
            Inactive = 2, 
            Deleted = 3 
        };
        
        Status status1 = Status::Pending;
        Status status2 = Status::Active;
        
        std::cout << "Status 1: " << static_cast<uint8_t>(status1) << std::endl;
        std::cout << "Status 2: " << static_cast<uint8_t>(status2) << std::endl;
    }

    // Enum class with methods and operators
    void enum_class_methods_examples() {
        std::cout << "\n=== Enum Class Methods Examples ===" << std::endl;
        
        // Enum class with associated functions
        enum class Month { 
            January = 1, February, March, April, May, June,
            July, August, September, October, November, December 
        };
        
        // Function to get month name
        auto get_month_name = [](Month month) -> std::string {
            switch (month) {
                case Month::January: return "January";
                case Month::February: return "February";
                case Month::March: return "March";
                case Month::April: return "April";
                case Month::May: return "May";
                case Month::June: return "June";
                case Month::July: return "July";
                case Month::August: return "August";
                case Month::September: return "September";
                case Month::October: return "October";
                case Month::November: return "November";
                case Month::December: return "December";
                default: return "Unknown";
            }
        };
        
        // Function to get days in month (simplified)
        auto get_days_in_month = [](Month month) -> int {
            switch (month) {
                case Month::February: return 28;
                case Month::April:
                case Month::June:
                case Month::September:
                case Month::November: return 30;
                default: return 31;
            }
        };
        
        Month current_month = Month::March;
        std::cout << "Current month: " << get_month_name(current_month) 
                  << " (" << static_cast<int>(current_month) << ")" << std::endl;
        std::cout << "Days in month: " << get_days_in_month(current_month) << std::endl;
        
        // Enum class with comparison operators
        enum class Priority { Low = 1, Medium = 2, High = 3, Critical = 4 };
        
        Priority p1 = Priority::Low;
        Priority p2 = Priority::High;
        
        std::cout << "Priority comparison:" << std::endl;
        std::cout << "p1 < p2: " << std::boolalpha << (p1 < p2) << std::endl;
        std::cout << "p1 == p2: " << (p1 == p2) << std::endl;
        std::cout << "p1 != p2: " << (p1 != p2) << std::endl;
    }

    // Enum class with containers
    void enum_class_container_examples() {
        std::cout << "\n=== Enum Class Container Examples ===" << std::endl;
        
        enum class Fruit { Apple, Banana, Orange, Grape, Mango };
        
        // Vector of enum class values
        std::vector<Fruit> fruits{Fruit::Apple, Fruit::Banana, Fruit::Orange};
        
        std::cout << "Fruits vector: ";
        for (const auto& fruit : fruits) {
            std::cout << static_cast<int>(fruit) << " ";
        }
        std::cout << std::endl;
        
        // Map with enum class as key
        std::map<Fruit, std::string> fruit_names{
            {Fruit::Apple, "Apple"},
            {Fruit::Banana, "Banana"},
            {Fruit::Orange, "Orange"},
            {Fruit::Grape, "Grape"},
            {Fruit::Mango, "Mango"}
        };
        
        std::cout << "Fruit names map:" << std::endl;
        for (const auto& [fruit, name] : fruit_names) {
            std::cout << "  " << static_cast<int>(fruit) << " -> " << name << std::endl;
        }
        
        // Map with enum class as value
        std::map<std::string, Fruit> name_to_fruit{
            {"apple", Fruit::Apple},
            {"banana", Fruit::Banana},
            {"orange", Fruit::Orange}
        };
        
        std::cout << "Name to fruit map:" << std::endl;
        for (const auto& [name, fruit] : name_to_fruit) {
            std::cout << "  " << name << " -> " << static_cast<int>(fruit) << std::endl;
        }
        
        // Set of enum class values
        std::set<Fruit> unique_fruits{Fruit::Apple, Fruit::Banana, Fruit::Apple, Fruit::Orange};
        
        std::cout << "Unique fruits set: ";
        for (const auto& fruit : unique_fruits) {
            std::cout << static_cast<int>(fruit) << " ";
        }
        std::cout << std::endl;
    }

    // Enum class for file permissions (defined outside function)
    enum class FilePermission : uint8_t {
        None = 0,
        Read = 1 << 0,      // 1
        Write = 1 << 1,     // 2
        Execute = 1 << 2    // 4
    };
    
    // Overload bitwise operators for enum class
    FilePermission operator|(FilePermission a, FilePermission b) {
        return static_cast<FilePermission>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }
    
    FilePermission operator&(FilePermission a, FilePermission b) {
        return static_cast<FilePermission>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }
    
    FilePermission operator~(FilePermission a) {
        return static_cast<FilePermission>(~static_cast<uint8_t>(a));
    }
    
    // Function to check if permission is set
    bool has_permission(FilePermission permissions, FilePermission check) {
        return (permissions & check) == check;
    }
    
    // Function to get permission string
    std::string get_permission_string(FilePermission permissions) {
        std::string result;
        result += has_permission(permissions, FilePermission::Read) ? "r" : "-";
        result += has_permission(permissions, FilePermission::Write) ? "w" : "-";
        result += has_permission(permissions, FilePermission::Execute) ? "x" : "-";
        return result;
    }

    // Enum class with bit flags
    void enum_class_bit_flags_examples() {
        std::cout << "\n=== Enum Class Bit Flags Examples ===" << std::endl;
        
        // Create different permission combinations
        FilePermission read_only = FilePermission::Read;
        FilePermission write_only = FilePermission::Write;
        FilePermission read_write = FilePermission::Read | FilePermission::Write;
        FilePermission all_permissions = FilePermission::Read | FilePermission::Write | FilePermission::Execute;
        
        std::cout << "Read only: " << get_permission_string(read_only) 
                  << " (" << static_cast<uint8_t>(read_only) << ")" << std::endl;
        std::cout << "Write only: " << get_permission_string(write_only) 
                  << " (" << static_cast<uint8_t>(write_only) << ")" << std::endl;
        std::cout << "Read write: " << get_permission_string(read_write) 
                  << " (" << static_cast<uint8_t>(read_write) << ")" << std::endl;
        std::cout << "All permissions: " << get_permission_string(all_permissions) 
                  << " (" << static_cast<uint8_t>(all_permissions) << ")" << std::endl;
        
        // Check specific permissions
        std::cout << "Read write has read: " << std::boolalpha 
                  << has_permission(read_write, FilePermission::Read) << std::endl;
        std::cout << "Read write has execute: " << has_permission(read_write, FilePermission::Execute) << std::endl;
    }

    // Enum class with type safety examples
    void type_safety_examples() {
        std::cout << "\n=== Type Safety Examples ===" << std::endl;
        
        // Two different enum classes
        enum class Color { Red, Green, Blue };
        enum class TrafficLight { Red, Yellow, Green };
        
        Color color = Color::Red;
        TrafficLight light = TrafficLight::Red;
        
        // These are different types - no implicit conversion
        // Color c = light;  // This would cause a compilation error
        // TrafficLight t = color;  // This would cause a compilation error
        
        std::cout << "Color Red: " << static_cast<int>(color) << std::endl;
        std::cout << "TrafficLight Red: " << static_cast<int>(light) << std::endl;
        
        // Function overloads work correctly
        auto process_color = [](Color c) -> std::string {
            return "Processing Color: " + std::to_string(static_cast<int>(c));
        };
        
        auto process_traffic_light = [](TrafficLight t) -> std::string {
            return "Processing TrafficLight: " + std::to_string(static_cast<int>(t));
        };
        
        std::cout << process_color(color) << std::endl;
        std::cout << process_traffic_light(light) << std::endl;
        
        // No implicit conversion to int
        // int color_value = color;  // This would cause a compilation error
        int color_value = static_cast<int>(color);  // Explicit conversion required
        std::cout << "Color value (explicit cast): " << color_value << std::endl;
    }

    // Enum class with scoping examples
    void scoping_examples() {
        std::cout << "\n=== Scoping Examples ===" << std::endl;
        
        // Enum class with scope
        enum class Animal { Dog, Cat, Bird, Fish };
        
        // Function that uses the enum
        auto get_animal_sound = [](Animal animal) -> std::string {
            switch (animal) {
                case Animal::Dog: return "Woof";
                case Animal::Cat: return "Meow";
                case Animal::Bird: return "Tweet";
                case Animal::Fish: return "Blub";
                default: return "Unknown";
            }
        };
        
        Animal pet = Animal::Dog;
        std::cout << "Animal: " << static_cast<int>(pet) << std::endl;
        std::cout << "Sound: " << get_animal_sound(pet) << std::endl;
        
        // Nested enum class
        class Game {
        public:
            enum class State { Menu, Playing, Paused, GameOver };
            enum class Difficulty { Easy, Medium, Hard };
            
            State current_state = State::Menu;
            Difficulty difficulty = Difficulty::Medium;
            
            void set_state(State state) {
                current_state = state;
            }
            
            void set_difficulty(Difficulty diff) {
                difficulty = diff;
            }
            
            std::string get_state_string() const {
                switch (current_state) {
                    case State::Menu: return "Menu";
                    case State::Playing: return "Playing";
                    case State::Paused: return "Paused";
                    case State::GameOver: return "Game Over";
                    default: return "Unknown";
                }
            }
            
            std::string get_difficulty_string() const {
                switch (difficulty) {
                    case Difficulty::Easy: return "Easy";
                    case Difficulty::Medium: return "Medium";
                    case Difficulty::Hard: return "Hard";
                    default: return "Unknown";
                }
            }
        };
        
        Game game;
        std::cout << "Game state: " << game.get_state_string() << std::endl;
        std::cout << "Game difficulty: " << game.get_difficulty_string() << std::endl;
        
        game.set_state(Game::State::Playing);
        game.set_difficulty(Game::Difficulty::Hard);
        
        std::cout << "After change - State: " << game.get_state_string() << std::endl;
        std::cout << "After change - Difficulty: " << game.get_difficulty_string() << std::endl;
    }

    // Enum class with serialization examples
    void serialization_examples() {
        std::cout << "\n=== Serialization Examples ===" << std::endl;
        
        enum class CardSuit { Hearts, Diamonds, Clubs, Spades };
        enum class CardRank { Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };
        
        // Convert enum to string
        auto suit_to_string = [](CardSuit suit) -> std::string {
            switch (suit) {
                case CardSuit::Hearts: return "Hearts";
                case CardSuit::Diamonds: return "Diamonds";
                case CardSuit::Clubs: return "Clubs";
                case CardSuit::Spades: return "Spades";
                default: return "Unknown";
            }
        };
        
        auto rank_to_string = [](CardRank rank) -> std::string {
            switch (rank) {
                case CardRank::Ace: return "Ace";
                case CardRank::Jack: return "Jack";
                case CardRank::Queen: return "Queen";
                case CardRank::King: return "King";
                default: return std::to_string(static_cast<int>(rank));
            }
        };
        
        // Convert string to enum
        auto string_to_suit = [](const std::string& str) -> CardSuit {
            if (str == "Hearts") return CardSuit::Hearts;
            if (str == "Diamonds") return CardSuit::Diamonds;
            if (str == "Clubs") return CardSuit::Clubs;
            if (str == "Spades") return CardSuit::Spades;
            return CardSuit::Hearts; // Default
        };
        
        auto string_to_rank = [](const std::string& str) -> CardRank {
            if (str == "Ace") return CardRank::Ace;
            if (str == "Jack") return CardRank::Jack;
            if (str == "Queen") return CardRank::Queen;
            if (str == "King") return CardRank::King;
            try {
                int value = std::stoi(str);
                if (value >= 2 && value <= 10) {
                    return static_cast<CardRank>(value);
                }
            } catch (...) {}
            return CardRank::Ace; // Default
        };
        
        // Create and serialize a card
        CardSuit suit = CardSuit::Hearts;
        CardRank rank = CardRank::Ace;
        
        std::string card_string = rank_to_string(rank) + " of " + suit_to_string(suit);
        std::cout << "Card: " << card_string << std::endl;
        
        // Deserialize
        std::string test_suit = "Spades";
        std::string test_rank = "King";
        
        CardSuit deserialized_suit = string_to_suit(test_suit);
        CardRank deserialized_rank = string_to_rank(test_rank);
        
        std::cout << "Deserialized card: " << rank_to_string(deserialized_rank) 
                  << " of " << suit_to_string(deserialized_suit) << std::endl;
    }

    // Template function that works with enum class
    template<typename T>
    auto get_enum_value(T enum_value) -> decltype(static_cast<std::underlying_type_t<T>>(enum_value)) {
        return static_cast<std::underlying_type_t<T>>(enum_value);
    }
    
    // Template class that works with enum class
    template<typename EnumType>
    class EnumWrapper {
    private:
        EnumType value_;
        
    public:
        EnumWrapper(EnumType value) : value_(value) {}
        
        EnumType get() const { return value_; }
        
        auto get_value() const -> decltype(static_cast<std::underlying_type_t<EnumType>>(value_)) {
            return static_cast<std::underlying_type_t<EnumType>>(value_);
        }
        
        bool operator==(const EnumWrapper& other) const {
            return value_ == other.value_;
        }
        
        bool operator<(const EnumWrapper& other) const {
            return get_value() < other.get_value();
        }
    };

    // Enum class with template examples
    void template_examples() {
        std::cout << "\n=== Template Examples ===" << std::endl;
        
        enum class Size { Small, Medium, Large };
        
        Size size = Size::Medium;
        std::cout << "Size value: " << get_enum_value(size) << std::endl;
        
        EnumWrapper<Size> wrapper(Size::Large);
        std::cout << "Wrapped size value: " << wrapper.get_value() << std::endl;
        
        // Vector of wrapped enums
        std::vector<EnumWrapper<Size>> sizes{
            EnumWrapper<Size>(Size::Small),
            EnumWrapper<Size>(Size::Medium),
            EnumWrapper<Size>(Size::Large)
        };
        
        std::cout << "Wrapped sizes: ";
        for (const auto& wrapped_size : sizes) {
            std::cout << wrapped_size.get_value() << " ";
        }
        std::cout << std::endl;
    }

    // Enum class with comparison and sorting
    void comparison_examples() {
        std::cout << "\n=== Comparison and Sorting Examples ===" << std::endl;
        
        enum class Grade { F = 0, D = 1, C = 2, B = 3, A = 4 };
        
        // Function to get grade string
        auto grade_to_string = [](Grade grade) -> std::string {
            switch (grade) {
                case Grade::F: return "F";
                case Grade::D: return "D";
                case Grade::C: return "C";
                case Grade::B: return "B";
                case Grade::A: return "A";
                default: return "Unknown";
            }
        };
        
        // Vector of grades
        std::vector<Grade> grades{Grade::B, Grade::A, Grade::C, Grade::F, Grade::D};
        
        std::cout << "Original grades: ";
        for (const auto& grade : grades) {
            std::cout << grade_to_string(grade) << " ";
        }
        std::cout << std::endl;
        
        // Sort grades (they will be sorted by underlying value)
        std::sort(grades.begin(), grades.end());
        
        std::cout << "Sorted grades: ";
        for (const auto& grade : grades) {
            std::cout << grade_to_string(grade) << " ";
        }
        std::cout << std::endl;
        
        // Find best and worst grades
        auto best_grade = *std::max_element(grades.begin(), grades.end());
        auto worst_grade = *std::min_element(grades.begin(), grades.end());
        
        std::cout << "Best grade: " << grade_to_string(best_grade) << std::endl;
        std::cout << "Worst grade: " << grade_to_string(worst_grade) << std::endl;
        
        // Count grades
        auto count_a_grades = std::count(grades.begin(), grades.end(), Grade::A);
        auto count_failing_grades = std::count_if(grades.begin(), grades.end(), 
            [](Grade g) { return g <= Grade::F; });
        
        std::cout << "Number of A grades: " << count_a_grades << std::endl;
        std::cout << "Number of failing grades: " << count_failing_grades << std::endl;
    }

    // Main demonstration function
    void demonstrate_strong_enums() {
        std::cout << "C++11 Strongly Typed Enums (Enum Class) Examples\n" << std::endl;
        
        basic_enum_class_examples();
        enum_class_methods_examples();
        enum_class_container_examples();
        enum_class_bit_flags_examples();
        type_safety_examples();
        scoping_examples();
        serialization_examples();
        template_examples();
        comparison_examples();
        
        std::cout << "\n=== Strongly Typed Enums Demonstration Complete ===" << std::endl;
    }

} // namespace strong_enum_examples

#endif // STRONG_ENUMS_HPP 