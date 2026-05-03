#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "calculator.hpp"

TEST_GROUP(CalculatorAddGroup)
{
};

TEST(CalculatorAddGroup, AddTwoIntegers)
{
    CHECK_EQUAL(5, calculator::add(2, 3));
    CHECK_EQUAL(0, calculator::add(-1, 1));
    CHECK_EQUAL(-5, calculator::add(-2, -3));
}

TEST(CalculatorAddGroup, AddTwoDoubles)
{
    DOUBLES_EQUAL(5.5, calculator::add(2.2, 3.3), 0.0001);
    DOUBLES_EQUAL(0.0, calculator::add(-1.5, 1.5), 0.0001);
}

TEST_GROUP(CalculatorSubtractGroup)
{
};

TEST(CalculatorSubtractGroup, SubtractTwoIntegers)
{
    CHECK_EQUAL(1, calculator::subtract(3, 2));
    CHECK_EQUAL(-5, calculator::subtract(2, 7));
    CHECK_EQUAL(0, calculator::subtract(5, 5));
}

TEST(CalculatorSubtractGroup, SubtractTwoDoubles)
{
    DOUBLES_EQUAL(1.1, calculator::subtract(5.5, 4.4), 0.0001);
    DOUBLES_EQUAL(-2.0, calculator::subtract(1.0, 3.0), 0.0001);
}

TEST_GROUP(CalculatorMultiplyGroup)
{
};

TEST(CalculatorMultiplyGroup, MultiplyTwoIntegers)
{
    CHECK_EQUAL(6, calculator::multiply(2, 3));
    CHECK_EQUAL(0, calculator::multiply(5, 0));
    CHECK_EQUAL(-6, calculator::multiply(-2, 3));
}

TEST(CalculatorMultiplyGroup, MultiplyTwoDoubles)
{
    DOUBLES_EQUAL(6.0, calculator::multiply(2.0, 3.0), 0.0001);
    DOUBLES_EQUAL(0.0, calculator::multiply(5.0, 0.0), 0.0001);
}

TEST_GROUP(CalculatorDivideGroup)
{
};

TEST(CalculatorDivideGroup, DivideTwoIntegers)
{
    CHECK_EQUAL(2, calculator::divide(6, 3));
    CHECK_EQUAL(0, calculator::divide(1, 3)); // integer division
}

TEST(CalculatorDivideGroup, DivideTwoDoubles)
{
    DOUBLES_EQUAL(2.5, calculator::divide(5.0, 2.0), 0.0001);
    DOUBLES_EQUAL(0.5, calculator::divide(1.0, 2.0), 0.0001);
}

TEST(CalculatorDivideGroup, DivideByZeroThrows)
{
    auto testInt = [] { (void)calculator::divide(5, 0); };
    CHECK_THROWS(std::invalid_argument, testInt());

    auto testDouble = [] { (void)calculator::divide(5.0, 0.0); };
    CHECK_THROWS(std::invalid_argument, testDouble());
}

TEST_GROUP(CalculatorModulusGroup)
{
};

TEST(CalculatorModulusGroup, ModulusIntegers)
{
    CHECK_EQUAL(1, calculator::modulus(7, 3));
    CHECK_EQUAL(0, calculator::modulus(6, 3));
}

TEST(CalculatorModulusGroup, ModulusByZeroThrows)
{
    auto testInt = [] { (void)calculator::modulus(5, 0); };
    CHECK_THROWS(std::invalid_argument, testInt());
}

TEST(CalculatorModulusGroup, ModulusDoubles)
{
    DOUBLES_EQUAL(1.5, calculator::modulus(5.5, 2.0), 0.0001);
}

TEST_GROUP(CalculatorPowerGroup)
{
};

TEST(CalculatorPowerGroup, PowerIntegers)
{
    CHECK_EQUAL(8, calculator::power(2, 3));
    CHECK_EQUAL(1, calculator::power(5, 0));
    CHECK_EQUAL(0, calculator::power(0, 5));
}

TEST(CalculatorPowerGroup, PowerDoubles)
{
    DOUBLES_EQUAL(8.0, calculator::power(2.0, 3.0), 0.0001);
    DOUBLES_EQUAL(4.0, calculator::power(16.0, 0.5), 0.0001); // square root
}

TEST(CalculatorPowerGroup, PowerNegativeExponentIntegralThrows)
{
    auto testInt = [] { (void)calculator::power(2, -1); };
    CHECK_THROWS(std::invalid_argument, testInt());
}

TEST_GROUP(CalculatorNegateGroup)
{
};

TEST(CalculatorNegateGroup, NegateIntegers)
{
    CHECK_EQUAL(-5, calculator::negate(5));
    CHECK_EQUAL(3, calculator::negate(-3));
    CHECK_EQUAL(0, calculator::negate(0));
}

TEST(CalculatorNegateGroup, NegateDoubles)
{
    DOUBLES_EQUAL(-2.5, calculator::negate(2.5), 0.0001);
    DOUBLES_EQUAL(3.14, calculator::negate(-3.14), 0.0001);
}

TEST_GROUP(CalculatorIsEqualGroup)
{
};

TEST(CalculatorIsEqualGroup, IsEqualIntegers)
{
    CHECK_TRUE(calculator::is_equal(5, 5));
    CHECK_FALSE(calculator::is_equal(5, 3));
}

TEST(CalculatorIsEqualGroup, IsEqualDoubles)
{
    CHECK_TRUE(calculator::is_equal(0.1 + 0.2, 0.3));
    CHECK_FALSE(calculator::is_equal(1.0, 2.0));
}

int main(int argc, char** argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
