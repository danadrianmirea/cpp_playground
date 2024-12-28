#pragma once

#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>

#define BIT0 (1 << 0) // 0x01   0000 0001
#define BIT1 (1 << 1) // 0x02   0000 0010
#define BIT2 (1 << 2) // 0x04   0000 0100
#define BIT3 (1 << 3) // 0x08   0000 1000
#define BIT4 (1 << 4) // 0x10   0001 0000
#define BIT5 (1 << 5) // 0x20   0010 0000
#define BIT6 (1 << 6) // 0x40   0100 0000
#define BIT7 (1 << 7) // 0x80   1000 0000

std::vector<int> convertToBinary(int num)
{
    std::vector<int> result;
    while (num)
    {
        result.push_back(num & 1);
        num = num >> 1;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string convertToBinaryStr(int num)
{
    std::string result;
    while (num)
    {
        result.push_back(num & 1);
        num = num >> 1;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

void testPrintInBinary(int i)
{
    auto res = convertToBinary(i);
    //for (auto& bit : res | std::views::reverse)
    //for (auto it = res.rbegin(); it != res.rend(); ++it)
    //{
        //std::cout << *it;
    //}
    for (auto& bit : res)
    {
        std::cout << bit;
    }
    std::cout << "\n";
}

void BitwiseOperationTests()
{
    int i = 0b00101100;
    std::cout << "i: " << std::bitset<8>(i) << std::endl;
    i ^= BIT2; // toggle bit2
    std::cout << "toggle bit2\n";
    std::cout << "i: " << std::bitset<8>(i) << std::endl;

    i &= (~BIT5); // unset bit 5
    std::cout << "unset bit 5\n";
    std::cout << "i: " << std::bitset<8>(i) << std::endl;

    i |= BIT4; // set bit 4
    i |= BIT2; // set bit 2. could also be done in one go: i |= (BIT4|BIT2)
    std::cout << "set bits 4 and 2\n";
    std::cout << "i: " << std::bitset<8>(i) << std::endl;

    i = ~i; // negate i
    std::cout << "negate i\n";
    std::cout << "i: " << std::bitset<8>(i) << std::endl;

    i &= (~(BIT7 | BIT6 | BIT5)); // unset bits 7 6 and 5 simultaneously
    std::cout << "unset bits 7 6 and 5 simultaneously\n";
    std::cout << "i: " << std::bitset<8>(i) << std::endl;

    i ^= (BIT7 | BIT6 | BIT5); // toggle bits 7, 6, 5
    std::cout << "toggle bits 7, 6, 5\n";
    std::cout << "i: " << std::bitset<8>(i) << std::endl;

    i = i & (~i); // set i to 0
    std::cout << "set i to 0\n";
    std::cout << "i: " << std::bitset<8>(i) << std::endl;

    i = 0b00101100;
    std::cout << "reset i to 0b00101100\n";
    std::cout << "i: " << std::bitset<8>(i) << std::endl;

    int j = 0b00001100;
    int k = 0b00001001;
    std::cout << "i: " << std::bitset<8>(i) << std::endl;
    std::cout << "j: " << std::bitset<8>(j) << std::endl;
    std::cout << "k: " << std::bitset<8>(k) << std::endl;
    if ((i & j) == j)
    {
        std::cout << "i has all the bits from j set\n";
    }
    if ((i & k) == k)
    {
        std::cout << "i has all the bits from k set\n";
    }

    int setmask = 0b10100110;
    std::cout << "i: " << std::bitset<8>(i) << std::endl;
    i |= setmask;
    std::cout << "i after setting bits from the mask: 0b10100000\n";
    std::cout << "i: " << std::bitset<8>(i) << std::endl;
}