#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <variant>
#include <functional>
#include <string>
#include <any>
#include <unordered_map>
#include <algorithm>
#include <utility>

namespace modern_polymorphic_patterns {

// 1. Traditional Virtual Inheritance Pattern
class TraditionalShape {
public:
    virtual ~TraditionalShape() = default;
    virtual void draw() const = 0;
    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public TraditionalShape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    void draw() const override { std::cout << "Drawing Circle with radius " << radius << std::endl; }
    double area() const override { return 3.14159 * radius * radius; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public TraditionalShape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    void draw() const override { std::cout << "Drawing Rectangle " << width << "x" << height << std::endl; }
    double area() const override { return width * height; }
    std::string name() const override { return "Rectangle"; }
};

// 2. std::variant Based Polymorphism
struct VariantCircle {
    double radius;
    void draw() const { std::cout << "Drawing Variant Circle with radius " << radius << std::endl; }
    double area() const { return 3.14159 * radius * radius; }
    std::string name() const { return "VariantCircle"; }
};

struct VariantRectangle {
    double width, height;
    void draw() const { std::cout << "Drawing Variant Rectangle " << width << "x" << height << std::endl; }
    double area() const { return width * height; }
    std::string name() const { return "VariantRectangle"; }
};

using VariantShape = std::variant<VariantCircle, VariantRectangle>;

// Generic visitor for variant shapes
template<typename Func>
void visit_shape(const VariantShape& shape, Func&& func) {
    std::visit(std::forward<Func>(func), shape);
}

// 3. Template-based Static Polymorphism (CRTP)
template<typename Derived>
class CRTPShape {
public:
    void draw() const {
        static_cast<const Derived*>(this)->draw_impl();
    }
    
    double area() const {
        return static_cast<const Derived*>(this)->area_impl();
    }
    
    std::string name() const {
        return static_cast<const Derived*>(this)->name_impl();
    }
};

class CRTPCircle : public CRTPShape<CRTPCircle> {
private:
    double radius;
public:
    CRTPCircle(double r) : radius(r) {}
    void draw_impl() const { std::cout << "Drawing CRTP Circle with radius " << radius << std::endl; }
    double area_impl() const { return 3.14159 * radius * radius; }
    std::string name_impl() const { return "CRTPCircle"; }
};

class CRTPRectangle : public CRTPShape<CRTPRectangle> {
private:
    double width, height;
public:
    CRTPRectangle(double w, double h) : width(w), height(h) {}
    void draw_impl() const { std::cout << "Drawing CRTP Rectangle " << width << "x" << height << std::endl; }
    double area_impl() const { return width * height; }
    std::string name_impl() const { return "CRTPRectangle"; }
};

// 4. Type Erasure Pattern
class TypeErasedShape {
private:
    struct ShapeInterface {
        virtual ~ShapeInterface() = default;
        virtual void draw() const = 0;
        virtual double area() const = 0;
        virtual std::string name() const = 0;
        virtual std::unique_ptr<ShapeInterface> clone() const = 0;
    };
    
    template<typename T>
    struct ShapeWrapper : ShapeInterface {
        T shape;
        
        ShapeWrapper(T s) : shape(std::move(s)) {}
        
        void draw() const override { shape.draw(); }
        double area() const override { return shape.area(); }
        std::string name() const override { return shape.name(); }
        
        std::unique_ptr<ShapeInterface> clone() const override {
            return std::make_unique<ShapeWrapper<T>>(shape);
        }
    };
    
    std::unique_ptr<ShapeInterface> pimpl;

public:
    template<typename T>
    TypeErasedShape(T shape) : pimpl(std::make_unique<ShapeWrapper<T>>(std::move(shape))) {}
    
    TypeErasedShape(const TypeErasedShape& other) : pimpl(other.pimpl->clone()) {}
    TypeErasedShape& operator=(const TypeErasedShape& other) {
        if (this != &other) {
            pimpl = other.pimpl->clone();
        }
        return *this;
    }
    
    TypeErasedShape(TypeErasedShape&&) = default;
    TypeErasedShape& operator=(TypeErasedShape&&) = default;
    
    void draw() const { pimpl->draw(); }
    double area() const { return pimpl->area(); }
    std::string name() const { return pimpl->name(); }
};

// 5. Concept-based Polymorphism (C++20)
template<typename T>
concept Drawable = requires(const T& t) {
    t.draw();
    { t.area() } -> std::convertible_to<double>;
    { t.name() } -> std::convertible_to<std::string>;
};

template<Drawable T>
void process_drawable(const T& drawable) {
    std::cout << "Processing " << drawable.name() << std::endl;
    drawable.draw();
    std::cout << "Area: " << drawable.area() << std::endl;
}

// Simple shapes for concept testing
struct ConceptCircle {
    double radius;
    void draw() const { std::cout << "Drawing Concept Circle with radius " << radius << std::endl; }
    double area() const { return 3.14159 * radius * radius; }
    std::string name() const { return "ConceptCircle"; }
};

struct ConceptRectangle {
    double width, height;
    void draw() const { std::cout << "Drawing Concept Rectangle " << width << "x" << height << std::endl; }
    double area() const { return width * height; }
    std::string name() const { return "ConceptRectangle"; }
};

// 6. Function Pointer Based Polymorphism
struct FunctionPtrShape {
    void* data;
    void (*draw_func)(const void*);
    double (*area_func)(const void*);
    std::string (*name_func)(const void*);
    
    template<typename T>
    FunctionPtrShape(const T& shape) : data(const_cast<void*>(static_cast<const void*>(&shape))) {
        draw_func = [](const void* ptr) {
            static_cast<const T*>(ptr)->draw();
        };
        area_func = [](const void* ptr) -> double {
            return static_cast<const T*>(ptr)->area();
        };
        name_func = [](const void* ptr) -> std::string {
            return static_cast<const T*>(ptr)->name();
        };
    }
    
    void draw() const { draw_func(data); }
    double area() const { return area_func(data); }
    std::string name() const { return name_func(data); }
};

// 7. std::function Based Polymorphism
class FunctionShape {
private:
    std::function<void()> draw_func;
    std::function<double()> area_func;
    std::function<std::string()> name_func;
    
public:
    template<typename T>
    FunctionShape(const T& shape) 
        : draw_func([&shape]() { shape.draw(); })
        , area_func([&shape]() { return shape.area(); })
        , name_func([&shape]() { return shape.name(); }) {}
    
    void draw() const { draw_func(); }
    double area() const { return area_func(); }
    std::string name() const { return name_func(); }
};

// 8. std::any Based Type Erasure
class AnyShape {
private:
    std::any data;
    std::function<void(const std::any&)> draw_func;
    std::function<double(const std::any&)> area_func;
    std::function<std::string(const std::any&)> name_func;
    
public:
    template<typename T>
    AnyShape(T shape) : data(std::move(shape)) {
        draw_func = [](const std::any& any_data) {
            std::any_cast<const T&>(any_data).draw();
        };
        area_func = [](const std::any& any_data) -> double {
            return std::any_cast<const T&>(any_data).area();
        };
        name_func = [](const std::any& any_data) -> std::string {
            return std::any_cast<const T&>(any_data).name();
        };
    }
    
    void draw() const { draw_func(data); }
    double area() const { return area_func(data); }
    std::string name() const { return name_func(data); }
};

// 9. Modern Visitor Pattern with std::visit and overloaded
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

// Enhanced variant shapes for visitor pattern
struct VisitorCircle {
    double radius;
    void draw() const { std::cout << "Drawing Visitor Circle with radius " << radius << std::endl; }
    double area() const { return 3.14159 * radius * radius; }
    std::string name() const { return "VisitorCircle"; }
};

struct VisitorRectangle {
    double width, height;
    void draw() const { std::cout << "Drawing Visitor Rectangle " << width << "x" << height << std::endl; }
    double area() const { return width * height; }
    std::string name() const { return "VisitorRectangle"; }
};

struct VisitorTriangle {
    double base, height;
    void draw() const { std::cout << "Drawing Visitor Triangle " << base << "x" << height << std::endl; }
    double area() const { return 0.5 * base * height; }
    std::string name() const { return "VisitorTriangle"; }
};

using VisitorShape = std::variant<VisitorCircle, VisitorRectangle, VisitorTriangle>;

// 10. Policy-Based Design Pattern
template<typename DrawingPolicy, typename CalculationPolicy>
class PolicyShape {
private:
    DrawingPolicy drawer;
    CalculationPolicy calculator;
    std::string shape_name;
    
public:
    PolicyShape(DrawingPolicy d, CalculationPolicy c, std::string name) 
        : drawer(std::move(d)), calculator(std::move(c)), shape_name(std::move(name)) {}
    
    void draw() const { drawer.draw(); }
    double area() const { return calculator.area(); }
    std::string name() const { return shape_name; }
};

// Drawing policies
struct ConsoleDrawer {
    std::string shape_info;
    ConsoleDrawer(std::string info) : shape_info(std::move(info)) {}
    void draw() const { std::cout << "Drawing: " << shape_info << std::endl; }
};

struct FileDrawer {
    std::string filename;
    FileDrawer(std::string file) : filename(std::move(file)) {}
    void draw() const { std::cout << "Drawing to file: " << filename << std::endl; }
};

// Calculation policies
struct CircleCalculator {
    double radius;
    CircleCalculator(double r) : radius(r) {}
    double area() const { return 3.14159 * radius * radius; }
};

struct RectangleCalculator {
    double width, height;
    RectangleCalculator(double w, double h) : width(w), height(h) {}
    double area() const { return width * height; }
};

// 11. Mixin Pattern for Composition
template<typename Base>
class DrawableMixin : public Base {
public:
    template<typename... Args>
    DrawableMixin(Args&&... args) : Base(std::forward<Args>(args)...) {}
    
    void draw() const {
        std::cout << "Drawing " << this->name() << " with area " << this->area() << std::endl;
    }
};

template<typename Base>
class ColoredMixin : public Base {
private:
    std::string color;
public:
    template<typename... Args>
    ColoredMixin(std::string c, Args&&... args) : Base(std::forward<Args>(args)...), color(std::move(c)) {}
    
    std::string get_color() const { return color; }
    void draw() const {
        std::cout << "Drawing " << this->name() << " in " << color << " color" << std::endl;
    }
};

// Base shape for mixins
struct MixinCircle {
    double radius;
    MixinCircle(double r) : radius(r) {}
    double area() const { return 3.14159 * radius * radius; }
    std::string name() const { return "MixinCircle"; }
};

// 12. Modern Observer Pattern
template<typename EventType>
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const EventType& event) = 0;
};

template<typename EventType>
class Subject {
private:
    std::vector<std::weak_ptr<Observer<EventType>>> observers;
    
public:
    void attach(std::shared_ptr<Observer<EventType>> observer) {
        observers.push_back(observer);
    }
    
    void detach(std::shared_ptr<Observer<EventType>> observer) {
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [&observer](const std::weak_ptr<Observer<EventType>>& weak_obs) {
                    return weak_obs.expired() || weak_obs.lock() == observer;
                }),
            observers.end());
    }
    
    void notify(const EventType& event) {
        for (auto it = observers.begin(); it != observers.end();) {
            if (auto observer = it->lock()) {
                observer->update(event);
                ++it;
            } else {
                it = observers.erase(it);
            }
        }
    }
};

// Event types for observer pattern
struct ShapeEvent {
    std::string shape_name;
    std::string event_type;
    double area;
};

// Helper struct for observer pattern demo
struct ShapeObserver : public Observer<ShapeEvent> {
    std::function<void(const ShapeEvent&)> update_func;
    void update(const ShapeEvent& event) override {
        if (update_func) update_func(event);
    }
};

// 13. Modern Factory Pattern with std::function
class ShapeFactory {
private:
    std::unordered_map<std::string, std::function<std::unique_ptr<TraditionalShape>(const std::vector<double>&)>> creators;
    
public:
    ShapeFactory() {
        register_shape("circle", [](const std::vector<double>& params) -> std::unique_ptr<TraditionalShape> {
            return std::make_unique<Circle>(params[0]);
        });
        
        register_shape("rectangle", [](const std::vector<double>& params) -> std::unique_ptr<TraditionalShape> {
            return std::make_unique<Rectangle>(params[0], params[1]);
        });
    }
    
    void register_shape(const std::string& name, 
                       std::function<std::unique_ptr<TraditionalShape>(const std::vector<double>&)> creator) {
        creators[name] = std::move(creator);
    }
    
    std::unique_ptr<TraditionalShape> create(const std::string& name, const std::vector<double>& params) {
        auto it = creators.find(name);
        if (it != creators.end()) {
            return it->second(params);
        }
        return nullptr;
    }
    
    std::vector<std::string> get_available_shapes() const {
        std::vector<std::string> names;
        for (const auto& pair : creators) {
            names.push_back(pair.first);
        }
        return names;
    }
};

// 14. Strategy Pattern with std::function
class ShapeRenderer {
private:
    std::function<void(const std::string&, double)> render_strategy;
    
public:
    ShapeRenderer(std::function<void(const std::string&, double)> strategy) 
        : render_strategy(std::move(strategy)) {}
    
    void render(const std::string& name, double area) const {
        render_strategy(name, area);
    }
    
    void set_strategy(std::function<void(const std::string&, double)> strategy) {
        render_strategy = std::move(strategy);
    }
};

// Render strategies
inline auto console_renderer = [](const std::string& name, double area) {
    std::cout << "Console: " << name << " has area " << area << std::endl;
};

inline auto json_renderer = [](const std::string& name, double area) {
    std::cout << "JSON: {\"name\":\"" << name << "\",\"area\":" << area << "}" << std::endl;
};

inline auto xml_renderer = [](const std::string& name, double area) {
    std::cout << "XML: <shape name=\"" << name << "\" area=\"" << area << "\"/>" << std::endl;
};

// Demo function
inline void demo_modern_polymorphic_patterns() {
    std::cout << "\n=== Modern Polymorphic Patterns Demo ===" << std::endl;

    // 1. Traditional Virtual Inheritance
    std::cout << "\n1. Traditional Virtual Inheritance:" << std::endl;
    std::vector<std::unique_ptr<TraditionalShape>> traditional_shapes;
    traditional_shapes.push_back(std::make_unique<Circle>(5.0));
    traditional_shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    
    for (const auto& shape : traditional_shapes) {
        std::cout << shape->name() << " - Area: " << shape->area() << std::endl;
        shape->draw();
    }

    // 2. std::variant Based Polymorphism
    std::cout << "\n2. std::variant Based Polymorphism:" << std::endl;
    std::vector<VariantShape> variant_shapes;
    variant_shapes.emplace_back(VariantCircle{3.0});
    variant_shapes.emplace_back(VariantRectangle{2.0, 8.0});
    
    for (const auto& shape : variant_shapes) {
        visit_shape(shape, [](const auto& s) {
            std::cout << s.name() << " - Area: " << s.area() << std::endl;
            s.draw();
        });
    }

    // 3. CRTP Static Polymorphism
    std::cout << "\n3. CRTP Static Polymorphism:" << std::endl;
    CRTPCircle crtp_circle(4.0);
    CRTPRectangle crtp_rect(3.0, 5.0);
    
    std::cout << crtp_circle.name() << " - Area: " << crtp_circle.area() << std::endl;
    crtp_circle.draw();
    std::cout << crtp_rect.name() << " - Area: " << crtp_rect.area() << std::endl;
    crtp_rect.draw();

    // 4. Type Erasure
    std::cout << "\n4. Type Erasure Pattern:" << std::endl;
    std::vector<TypeErasedShape> erased_shapes;
    erased_shapes.emplace_back(ConceptCircle{6.0});
    erased_shapes.emplace_back(ConceptRectangle{4.0, 7.0});
    
    for (const auto& shape : erased_shapes) {
        std::cout << shape.name() << " - Area: " << shape.area() << std::endl;
        shape.draw();
    }

    // 5. Concept-based Polymorphism
    std::cout << "\n5. Concept-based Polymorphism:" << std::endl;
    ConceptCircle concept_circle{2.5};
    ConceptRectangle concept_rect{1.5, 9.0};
    
    process_drawable(concept_circle);
    process_drawable(concept_rect);

    // 6. Function Pointer Based
    std::cout << "\n6. Function Pointer Based Polymorphism:" << std::endl;
    ConceptCircle fp_circle{3.5};
    ConceptRectangle fp_rect{2.5, 4.5};
    
    std::vector<FunctionPtrShape> fp_shapes;
    fp_shapes.emplace_back(fp_circle);
    fp_shapes.emplace_back(fp_rect);
    
    for (const auto& shape : fp_shapes) {
        std::cout << shape.name() << " - Area: " << shape.area() << std::endl;
        shape.draw();
    }

    // 7. std::function Based Polymorphism
    std::cout << "\n7. std::function Based Polymorphism:" << std::endl;
    ConceptCircle func_circle{4.5};
    ConceptRectangle func_rect{3.5, 6.5};
    
    std::vector<FunctionShape> func_shapes;
    func_shapes.emplace_back(func_circle);
    func_shapes.emplace_back(func_rect);
    
    for (const auto& shape : func_shapes) {
        std::cout << shape.name() << " - Area: " << shape.area() << std::endl;
        shape.draw();
    }

    // 8. std::any Based Type Erasure
    std::cout << "\n8. std::any Based Type Erasure:" << std::endl;
    std::vector<AnyShape> any_shapes;
    any_shapes.emplace_back(ConceptCircle{5.5});
    any_shapes.emplace_back(ConceptRectangle{4.5, 8.5});
    
    for (const auto& shape : any_shapes) {
        std::cout << shape.name() << " - Area: " << shape.area() << std::endl;
        shape.draw();
    }

    // 9. Modern Visitor Pattern
    std::cout << "\n9. Modern Visitor Pattern:" << std::endl;
    std::vector<VisitorShape> visitor_shapes;
    visitor_shapes.emplace_back(VisitorCircle{6.5});
    visitor_shapes.emplace_back(VisitorRectangle{5.5, 9.5});
    visitor_shapes.emplace_back(VisitorTriangle{4.0, 7.0});
    
    auto visitor = overloaded{
        [](const VisitorCircle& c) {
            std::cout << "Visiting " << c.name() << " with radius " << c.radius << std::endl;
            c.draw();
        },
        [](const VisitorRectangle& r) {
            std::cout << "Visiting " << r.name() << " " << r.width << "x" << r.height << std::endl;
            r.draw();
        },
        [](const VisitorTriangle& t) {
            std::cout << "Visiting " << t.name() << " " << t.base << "x" << t.height << std::endl;
            t.draw();
        }
    };
    
    for (const auto& shape : visitor_shapes) {
        std::visit(visitor, shape);
    }

    // 10. Policy-Based Design
    std::cout << "\n10. Policy-Based Design:" << std::endl;
    auto console_circle = PolicyShape(
        ConsoleDrawer("Circle with radius 7.0"),
        CircleCalculator(7.0),
        "PolicyCircle"
    );
    
    auto file_rectangle = PolicyShape(
        FileDrawer("rectangle.txt"),
        RectangleCalculator(6.0, 10.0),
        "PolicyRectangle"
    );
    
    console_circle.draw();
    std::cout << "Area: " << console_circle.area() << std::endl;
    
    file_rectangle.draw();
    std::cout << "Area: " << file_rectangle.area() << std::endl;

    // 11. Mixin Pattern
    std::cout << "\n11. Mixin Pattern:" << std::endl;
    using ColoredDrawableCircle = ColoredMixin<DrawableMixin<MixinCircle>>;
    ColoredDrawableCircle colored_circle("red", 8.0);
    colored_circle.draw();
    std::cout << "Color: " << colored_circle.get_color() << std::endl;

    // 12. Observer Pattern
    std::cout << "\n12. Observer Pattern:" << std::endl;
    Subject<ShapeEvent> shape_subject;
    
    auto observer1 = std::make_shared<ShapeObserver>();
    observer1->update_func = [](const ShapeEvent& event) {
        std::cout << "Observer1: " << event.event_type << " for " << event.shape_name 
                  << " (area: " << event.area << ")" << std::endl;
    };
    
    auto observer2 = std::make_shared<ShapeObserver>();
    observer2->update_func = [](const ShapeEvent& event) {
        std::cout << "Observer2: Logging " << event.shape_name << " event" << std::endl;
    };
    
    shape_subject.attach(observer1);
    shape_subject.attach(observer2);
    
    ShapeEvent event{"Circle", "created", 78.54};
    shape_subject.notify(event);

    // 13. Factory Pattern
    std::cout << "\n13. Factory Pattern:" << std::endl;
    ShapeFactory factory;
    
    auto shapes = factory.get_available_shapes();
    std::cout << "Available shapes: ";
    for (const auto& name : shapes) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
    
    auto circle = factory.create("circle", {9.0});
    auto rectangle = factory.create("rectangle", {7.0, 11.0});
    
    if (circle) {
        std::cout << "Created: " << circle->name() << " - Area: " << circle->area() << std::endl;
        circle->draw();
    }
    
    if (rectangle) {
        std::cout << "Created: " << rectangle->name() << " - Area: " << rectangle->area() << std::endl;
        rectangle->draw();
    }

    // 14. Strategy Pattern
    std::cout << "\n14. Strategy Pattern:" << std::endl;
    ShapeRenderer renderer(console_renderer);
    renderer.render("Circle", 78.54);
    
    renderer.set_strategy(json_renderer);
    renderer.render("Rectangle", 77.0);
    
    renderer.set_strategy(xml_renderer);
    renderer.render("Triangle", 14.0);

    std::cout << "\n=== Comparison of Patterns ===" << std::endl;
    std::cout << "1. Virtual Inheritance: Runtime polymorphism, dynamic dispatch, inheritance required" << std::endl;
    std::cout << "2. std::variant: Compile-time polymorphism, type-safe union, visitor pattern" << std::endl;
    std::cout << "3. CRTP: Compile-time polymorphism, static dispatch, template-based" << std::endl;
    std::cout << "4. Type Erasure: Runtime polymorphism, no inheritance required, value semantics" << std::endl;
    std::cout << "5. Concepts: Compile-time constraints, duck typing, template-based" << std::endl;
    std::cout << "6. Function Pointers: Runtime polymorphism, C-style, manual vtable" << std::endl;
    std::cout << "7. std::function: Runtime polymorphism, callable objects, flexible" << std::endl;
    std::cout << "8. std::any: Runtime type erasure, any type storage, type-safe casting" << std::endl;
    std::cout << "9. Visitor: Compile-time dispatch, extensible operations, std::visit" << std::endl;
    std::cout << "10. Policy-based: Compile-time composition, template policies, flexible design" << std::endl;
    std::cout << "11. Mixin: Compile-time composition, multiple inheritance alternative" << std::endl;
    std::cout << "12. Observer: Runtime notification, event-driven, loose coupling" << std::endl;
    std::cout << "13. Factory: Runtime creation, registration-based, extensible" << std::endl;
    std::cout << "14. Strategy: Runtime algorithm selection, std::function-based, flexible" << std::endl;
}

} // namespace modern_polymorphic_patterns
