#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <variant>
#include <functional>
#include <string>
#include <type_traits>

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

    std::cout << "\n=== Comparison of Patterns ===" << std::endl;
    std::cout << "1. Virtual Inheritance: Runtime polymorphism, dynamic dispatch, inheritance required" << std::endl;
    std::cout << "2. std::variant: Compile-time polymorphism, type-safe union, visitor pattern" << std::endl;
    std::cout << "3. CRTP: Compile-time polymorphism, static dispatch, template-based" << std::endl;
    std::cout << "4. Type Erasure: Runtime polymorphism, no inheritance required, value semantics" << std::endl;
    std::cout << "5. Concepts: Compile-time constraints, duck typing, template-based" << std::endl;
    std::cout << "6. Function Pointers: Runtime polymorphism, C-style, manual vtable" << std::endl;
}

} // namespace modern_polymorphic_patterns
