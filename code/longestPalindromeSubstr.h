#pragma once

#include <algorithm>
#include <string>


using namespace std;

string longestPalindrome(string s) {
  int n = s.size();
  if (n <= 1)
    return s;

  for (int size = n; size >= 1; --size) {
    for (int i = 0; i + size <= n; ++i) {
      int left = i, right = i + size - 1;
      bool isPalindrome = true;

      while (left < right) {
        if (s[left] != s[right]) {
          isPalindrome = false;
          break;
        }
        ++left;
        --right;
      }

      if (isPalindrome) {
        return s.substr(i, size);
      }
    }
  }
  return {};
}