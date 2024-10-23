#include "generictest.hpp"
#include "staticmember.h"

#include <iostream>
using namespace std;

void GenericTest::Test()
{
    cout << "From GenericTest::Test(), StaticMember::x = " << StaticMember::x << endl;
}