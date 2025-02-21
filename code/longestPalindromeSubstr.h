#pragma once

#include <string>

using namespace std;

string longestPalindrome(string s)
{
    int n = s.size();
    if (n <= 1) return s;

    for (int size = n; size >= 1; --size)
    {
        for (int i = 0; i + size <= n; ++i)
        {
            string sub = s.substr(i, size);
            string rev = sub;
            reverse(rev.begin(), rev.end());

            if (sub == rev)
            {
                return sub;
            }
        }
    }
    return {};
}