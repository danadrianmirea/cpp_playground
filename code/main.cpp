#include <iostream>
#include <vector>

using namespace std;

void Permute(std::vector<int> &v, int l, int r) {
  if (l == r) {
    for (int num : v) {
      std::cout << num << " ";
    }
    std::cout << std::endl;
  } else {
    for (int i = l; i <= r; i++) {
      std::swap(v[l], v[i]);
      Permute(v, l + 1, r);
      std::swap(v[l], v[i]);
    }
  }
}

void GeneratePermutationsRec(std::vector<int> &v) {
  Permute(v, 0, v.size() - 1);
}

void GeneratePermutationsIter(std::vector<int> &v) {
  int n = v.size();
  std::vector<int> c(n, 0); // Control array for swaps (initialized to 0)

  // Print the initial permutation
  for (int num : v) {
    std::cout << num << " ";
  }
  std::cout << std::endl;

  int i = 0;
  while (i < n) {
    if (c[i] < i) {
      if (i % 2 == 0) {
        std::swap(v[0], v[i]);
      } else {
        std::swap(v[c[i]], v[i]);
      }

      // Print the permutation
      for (int num : v) {
        std::cout << num << " ";
      }
      std::cout << std::endl;

      c[i]++; // Increase counter for position i
      i = 0;  // Reset i to generate new permutation
    } else {
      c[i] = 0; // Reset counter for position i
      i++;      // Move to the next index
    }
  }
}

int main() {
  std::vector<int> v = {1, 2, 3};
  // GeneratePermutationsRec(v);
  GeneratePermutationsIter(v);
}