#pragma once

#include <iostream>
#include <iomanip>
#include <cmath>
#include <numbers>
#include <complex>
#include <vector>
#include <algorithm>
#include <numeric>

namespace stdNumbersDemo {

    // Helper function to print constant with high precision
    template<typename T>
    void print_constant(const std::string& name, T value, int precision = 15) {
        std::cout << std::setw(20) << name << ": " 
                  << std::setprecision(precision) << std::fixed << value << std::endl;
    }

    // 1. Basic mathematical constants
    void demo_basic_constants() {
        std::cout << "\n=== Basic Mathematical Constants ===" << std::endl;
        
        print_constant("π (pi)", std::numbers::pi);
        print_constant("e (Euler's number)", std::numbers::e);
        print_constant("φ (golden ratio)", std::numbers::phi);
        print_constant("ln(2)", std::numbers::ln2);
        print_constant("ln(10)", std::numbers::ln10);
        print_constant("log₂(e)", std::numbers::log2e);
        print_constant("log₁₀(e)", std::numbers::log10e);
        print_constant("√2", std::numbers::sqrt2);
        print_constant("√3", std::numbers::sqrt3);
        print_constant("1/√2", 1.0 / std::numbers::sqrt2);
        print_constant("1/√3", 1.0 / std::numbers::sqrt3);
        print_constant("e^γ (Euler-Mascheroni)", std::numbers::egamma);
    }

    // 2. Different precision types
    void demo_precision_types() {
        std::cout << "\n=== Constants with Different Precision Types ===" << std::endl;
        
        std::cout << "\nFloat precision:" << std::endl;
        print_constant("π (float)", std::numbers::pi_v<float>);
        print_constant("e (float)", std::numbers::e_v<float>);
        print_constant("φ (float)", std::numbers::phi_v<float>);
        
        std::cout << "\nDouble precision:" << std::endl;
        print_constant("π (double)", std::numbers::pi_v<double>);
        print_constant("e (double)", std::numbers::e_v<double>);
        print_constant("φ (double)", std::numbers::phi_v<double>);
        
        std::cout << "\nLong double precision:" << std::endl;
        print_constant("π (long double)", std::numbers::pi_v<long double>);
        print_constant("e (long double)", std::numbers::e_v<long double>);
        print_constant("φ (long double)", std::numbers::phi_v<long double>);
    }

    // 3. Practical applications of mathematical constants
    void demo_practical_applications() {
        std::cout << "\n=== Practical Applications ===" << std::endl;
        
        // Circle calculations
        double radius = 5.0;
        double area = std::numbers::pi * radius * radius;
        double circumference = 2 * std::numbers::pi * radius;
        
        std::cout << "\nCircle with radius " << radius << ":" << std::endl;
        print_constant("Area", area);
        print_constant("Circumference", circumference);
        
        // Exponential growth/decay
        double time = 2.0;
        double growth_rate = 0.1;
        double initial_amount = 100.0;
        double final_amount = initial_amount * std::pow(std::numbers::e, growth_rate * time);
        
        std::cout << "\nExponential growth:" << std::endl;
        std::cout << "Initial amount: " << initial_amount << std::endl;
        std::cout << "Growth rate: " << growth_rate << " per unit time" << std::endl;
        std::cout << "Time: " << time << " units" << std::endl;
        print_constant("Final amount", final_amount);
        
        // Golden ratio applications
        double a = 10.0;
        double b = a / std::numbers::phi;
        double c = a + b;
        
        std::cout << "\nGolden ratio proportions:" << std::endl;
        print_constant("Segment A", a);
        print_constant("Segment B", b);
        print_constant("Total (A+B)", c);
        print_constant("Ratio (A+B)/A", c / a);
        print_constant("Ratio A/B", a / b);
        
        // Logarithmic calculations
        double number = 1000.0;
        double log2_result = std::log2(number);
        double log10_result = std::log10(number);
        double ln_result = std::log(number);
        
        std::cout << "\nLogarithmic calculations for " << number << ":" << std::endl;
        print_constant("log₂(x)", log2_result);
        print_constant("log₁₀(x)", log10_result);
        print_constant("ln(x)", ln_result);
    }

    // 4. Complex number calculations
    void demo_complex_calculations() {
        std::cout << "\n=== Complex Number Calculations ===" << std::endl;
        
        // Euler's formula: e^(iπ) = -1
        std::complex<double> euler_formula = std::pow(std::numbers::e, 
                                                     std::complex<double>(0, std::numbers::pi));
        
        std::cout << "Euler's formula: e^(iπ) = " << euler_formula << std::endl;
        std::cout << "Expected result: -1 + 0i" << std::endl;
        
        // Complex exponential
        std::complex<double> z(1.0, std::numbers::pi / 4);
        std::complex<double> exp_z = std::exp(z);
        
        std::cout << "\nComplex exponential:" << std::endl;
        std::cout << "z = " << z << std::endl;
        std::cout << "e^z = " << exp_z << std::endl;
        
        // Complex logarithm
        std::complex<double> w(2.0, 3.0);
        std::complex<double> log_w = std::log(w);
        
        std::cout << "\nComplex logarithm:" << std::endl;
        std::cout << "w = " << w << std::endl;
        std::cout << "ln(w) = " << log_w << std::endl;
    }

    // 5. Trigonometric calculations
    void demo_trigonometric_calculations() {
        std::cout << "\n=== Trigonometric Calculations ===" << std::endl;
        
        // Common angles in radians
        double angle_30 = std::numbers::pi / 6;
        double angle_45 = std::numbers::pi / 4;
        double angle_60 = std::numbers::pi / 3;
        double angle_90 = std::numbers::pi / 2;
        
        std::cout << "\nCommon angles and their trigonometric values:" << std::endl;
        std::cout << std::setw(10) << "Angle" << std::setw(15) << "sin" 
                  << std::setw(15) << "cos" << std::setw(15) << "tan" << std::endl;
        
        auto print_trig_values = [](double angle, const std::string& name) {
            std::cout << std::setw(10) << name 
                      << std::setw(15) << std::sin(angle)
                      << std::setw(15) << std::cos(angle)
                      << std::setw(15) << std::tan(angle) << std::endl;
        };
        
        print_trig_values(angle_30, "30°");
        print_trig_values(angle_45, "45°");
        print_trig_values(angle_60, "60°");
        print_trig_values(angle_90, "90°");
        
        // Inverse trigonometric functions
        double value = 0.5;
        std::cout << "\nInverse trigonometric functions for " << value << ":" << std::endl;
        print_constant("arcsin(0.5)", std::asin(value));
        print_constant("arccos(0.5)", std::acos(value));
        print_constant("arctan(0.5)", std::atan(value));
    }

    // 6. Statistical calculations using constants
    void demo_statistical_calculations() {
        std::cout << "\n=== Statistical Calculations ===" << std::endl;
        
        // Normal distribution calculations
        double mean = 0.0;
        double std_dev = 1.0;
        double x = 1.0;
        
        // Probability density function of normal distribution
        double pdf = (1.0 / (std_dev * std::sqrt(2 * std::numbers::pi))) *
                     std::exp(-0.5 * std::pow((x - mean) / std_dev, 2));
        
        std::cout << "\nNormal distribution (mean=0, std_dev=1):" << std::endl;
        print_constant("PDF at x=1", pdf);
        
        // Poisson distribution
        double lambda = 3.0;
        int k = 2;
        double poisson_pmf = (std::pow(lambda, k) * std::exp(-lambda)) / 
                            std::tgamma(k + 1);
        
        std::cout << "\nPoisson distribution (λ=3, k=2):" << std::endl;
        print_constant("PMF", poisson_pmf);
        
        // Geometric series sum
        double r = 0.5;
        int n = 10;
        double geometric_sum = (1 - std::pow(r, n)) / (1 - r);
        
        std::cout << "\nGeometric series (r=0.5, n=10):" << std::endl;
        print_constant("Sum", geometric_sum);
    }

    // 7. Numerical methods using constants
    void demo_numerical_methods() {
        std::cout << "\n=== Numerical Methods ===" << std::endl;
        
        // Newton's method for finding square root
        auto newton_sqrt = [](double x, int iterations = 10) {
            double guess = x / 2.0;
            for (int i = 0; i < iterations; ++i) {
                guess = 0.5 * (guess + x / guess);
            }
            return guess;
        };
        
        double number = 2.0;
        double newton_result = newton_sqrt(number);
        double std_result = std::sqrt(number);
        
        std::cout << "\nNewton's method for √" << number << ":" << std::endl;
        print_constant("Newton's result", newton_result);
        print_constant("std::sqrt result", std_result);
        print_constant("Difference", std::abs(newton_result - std_result));
        
        // Monte Carlo estimation of π
        std::cout << "\nMonte Carlo estimation of π:" << std::endl;
        int samples = 1000000;
        int inside_circle = 0;
        
        std::srand(42); // Fixed seed for reproducibility
        for (int i = 0; i < samples; ++i) {
            double x = static_cast<double>(std::rand()) / RAND_MAX;
            double y = static_cast<double>(std::rand()) / RAND_MAX;
            if (x * x + y * y <= 1.0) {
                inside_circle++;
            }
        }
        
        double pi_estimate = 4.0 * inside_circle / samples;
        print_constant("Monte Carlo π", pi_estimate);
        print_constant("Actual π", std::numbers::pi);
        print_constant("Error", std::abs(pi_estimate - std::numbers::pi));
    }

    // 8. Custom constant calculations
    void demo_custom_constants() {
        std::cout << "\n=== Custom Constant Calculations ===" << std::endl;
        
        // Calculate some interesting mathematical constants
        double catalan = 0.0;
        for (int n = 0; n < 20; ++n) {
            catalan += std::pow(-1, n) / std::pow(2 * n + 1, 2);
        }
        
        double apéry = 0.0;
        for (int n = 1; n <= 1000; ++n) {
            apéry += 1.0 / std::pow(n, 3);
        }
        
        double euler_mascheroni = 0.0;
        for (int n = 1; n <= 100000; ++n) {
            euler_mascheroni += 1.0 / n;
        }
        euler_mascheroni -= std::log(100000);
        
        print_constant("Catalan constant", catalan);
        print_constant("Apéry's constant", apéry);
        print_constant("Euler-Mascheroni", euler_mascheroni);
        
        // Calculate some ratios
        double pi_e_ratio = std::numbers::pi / std::numbers::e;
        double e_pi_ratio = std::numbers::e / std::numbers::pi;
        double phi_pi_ratio = std::numbers::phi / std::numbers::pi;
        
        std::cout << "\nInteresting ratios:" << std::endl;
        print_constant("π/e", pi_e_ratio);
        print_constant("e/π", e_pi_ratio);
        print_constant("φ/π", phi_pi_ratio);
    }

    // Main demonstration function
    void demo_std_numbers() {
        std::cout << "=== C++20 std::numbers Mathematical Constants Demo ===" << std::endl;
        std::cout << "Demonstrating mathematical constants and their applications" << std::endl;
        
        demo_basic_constants();
        demo_precision_types();
        demo_practical_applications();
        demo_complex_calculations();
        demo_trigonometric_calculations();
        demo_statistical_calculations();
        demo_numerical_methods();
        demo_custom_constants();
        
        std::cout << "\n=== Mathematical Constants Summary ===" << std::endl;
        std::cout << "1. π (pi): Circle circumference to diameter ratio" << std::endl;
        std::cout << "2. e: Base of natural logarithm, exponential growth" << std::endl;
        std::cout << "3. φ (phi): Golden ratio, appears in nature and art" << std::endl;
        std::cout << "4. ln2, ln10: Natural logarithms of 2 and 10" << std::endl;
        std::cout << "5. log2e, log10e: Logarithms of e in different bases" << std::endl;
        std::cout << "6. √2, √3: Square roots of 2 and 3" << std::endl;
        std::cout << "7. inv_sqrt2, inv_sqrt3: Inverse square roots" << std::endl;
        std::cout << "8. egamma: Euler-Mascheroni constant" << std::endl;
        
        std::cout << "\n=== std::numbers Demo Complete ===" << std::endl;
    }

} // namespace stdNumbersDemo
