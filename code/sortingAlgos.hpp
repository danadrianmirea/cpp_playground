#pragma once

#include <sys/time.h>

#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>


namespace adi_sorting_algos
{
    inline void swap(int &a, int &b)
    {
        int temp = a;
        a = b;
        b = temp;
    }

    inline void bubble_sort(std::vector<int>& v)
    {
        int n = v.size();
        for (int end = n - 1; end > 0; end--)
        {
            for (int j = 0; j < end; j++)
            {
                if (v[j] > v[j + 1])
                {
                    std::swap(v[j], v[j + 1]);
                }
            }
        }
    }

    inline void selection_sort(std::vector<int>& v)
    {
        int startIndex = 0;
        int n = v.size();
        int minIndex;

        for (int startIndex = 0; startIndex < n - 1; ++startIndex)
        {
            // select smallest item
            minIndex = startIndex;
            for (int i = startIndex + 1; i < n; ++i)
            {
                if (v[i] < v[minIndex])
                {
                    minIndex = i;
                }
            }

            // put it at the start
            if (startIndex != minIndex)
            {
                std::swap(v[startIndex], v[minIndex]);
            }
        }
    }   

    inline void sort_by_insertion(std::vector<int> &v)
    {
        if (v.size() < 2)
        {
            return;
        }

        int start = 0;
        int minPos;
        int minValue;

        while (start < v.size())
        {
            minPos = start;
            minValue = v[start];

            for (int i = start + 1; i < v.size(); i++)
            {
                if (minValue > v[i])
                {
                    minPos = i;
                    minValue = v[i];
                }
            }

            if (minPos != start)
            {
                swap(v[start], v[minPos]);
            }

            start++;
        }
    }

    bool is_sorted(std::vector<int>& v)
    {
        int n = v.size();
        for (int i = 1; i < n; i++)
        {
            if (v[i] < v[i - 1])
            {
                return false;
            }
        }
        return true;
    }

    // monkey sort, also known as bogosort
    void monkey_sort(std::vector<int>& v)
    {
        std::random_device rd;
        std::mt19937 rng(rd());

        while (is_sorted(v) == false)
        {
            std::shuffle(v.begin(), v.end(), rng);
        }
    }

    void random_swap_sort(std::vector<int>& v)
    {
        static bool is_seeded = false;

        int n = v.size();

        if (is_seeded == false)
        {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            srand(tv.tv_usec);
            is_seeded = true;
        }

        while (!is_sorted(v))
        {
            int index1 = rand() % n;
            int index2 = rand() % n;

            while (index1 == index2)
            {
                index2 = rand() % n;
            }
            std::swap(v[index1], v[index2]);
        }
    }

    inline void insertion_sort_v2(std::vector<int>& v)
    {
        int n = v.size();
        for (int i = 1; i < n; ++i)
        {
            int tmp = v[i];
            int cInd = i;
            while (cInd > 0 && tmp < v[cInd - 1])
            {
                v[cInd] = v[cInd - 1];
                cInd--;
            }
            v[cInd] = tmp;
        }
    }    

    inline int testInsertionSort()
    {
        std::cout << "Program start: \n";

        srand(time(0));
        std::vector<int> v;

        const int size = 7;

        for (int i = 0; i < size; i++)
        {
            v.push_back(rand() % 256);
        }

        std::cout << "Initial Vector: \n";
        for (auto e : v)
        {
            std::cout << e << " ";
        }
        std::cout << "\n";

        sort_by_insertion(v);

        std::cout << "Sorted Vector: \n";
        for (auto e : v)
        {
            std::cout << e << " ";
        }
        std::cout << "\n";

        return 0;
    }


    // merges two already sorted vectors
    void merge(std::vector<int>& v, int left, int mid, int right)
    {
        int n1 = (mid - left) + 1;
        int n2 = (right - mid);

        std::vector<int> leftVector(n1);
        std::vector<int> rightVector(n2);

        for (int i = 0; i < n1; ++i)
        {
            leftVector[i] = v[left + i];
        }

        for (int i = 0; i < n2; ++i)
        {
            rightVector[i] = v[mid + 1 + i];
        }

        int leftIndex = 0;
        int rightIndex = 0;
        int cIndex = left;

        while (leftIndex < n1 && rightIndex < n2)
        {
            if (leftVector[leftIndex] < rightVector[rightIndex])
            {
                v[cIndex] = leftVector[leftIndex];
                leftIndex++;
                cIndex++;
            }
            else
            {
                v[cIndex] = rightVector[rightIndex];
                rightIndex++;
                cIndex++;
            }
        }

        while (leftIndex < n1)
        {
            v[cIndex] = leftVector[leftIndex];
            leftIndex++;
            cIndex++;
        }

        while (rightIndex < n2)
        {
            v[cIndex] = rightVector[rightIndex];
            rightIndex++;
            cIndex++;
        }
        }

    // recursively split the array and merge the bits
    void merge_sort_split(std::vector<int>& v, int left, int right)
    {
        int mid = left + (right - left) / 2;

        if (left < right)
        {
            merge_sort_split(v, left, mid);
            merge_sort_split(v, mid + 1, right);
            merge(v, left, mid, right);
        }
    }

    void merge_sort_iter(std::vector<int>& v)
    {
        int n = v.size();
        for (int cSize = 1; cSize < n; cSize *= 2)
        {
            for (int left = 0; left < n - 1; left += 2 * cSize)
            {
                int right = std::min(left + 2 * cSize - 1, n - 1);
                int mid = std::min(left + cSize - 1, n - 1);
                merge(v, left, mid, right);
            }
        }
    }

    inline void merge_sort(std::vector<int>& v)
    {
        merge_sort_split(v, 0, v.size() - 1);
    }

    std::vector<int> generate_random_vector(int n, int lower_bound, int upper_bound)
    {
        // Random number generator setup
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(lower_bound, upper_bound);

        std::vector<int> v(n);
        for (int i = 0; i < n; ++i)
        {
            v[i] = dis(gen);  // Fill the vector with random values in the given range
        }

        return v;
    }

    int quicksort_partition(std::vector<int>&v, int left, int right)
    {
        int pPos = left;
        for(int i=left; i<right; ++i)
        {
            if(v[i]<v[right])
            {
                std::swap(v[i], v[pPos]);
                pPos++;
            }
        }
            std::swap(v[right], v[pPos]);
            return pPos;
    }

    void quicksort(std::vector<int>& v, int left, int right)
    {
        if(left>=right) return;
        int pPos = quicksort_partition(v, left, right);
        quicksort(v, left, pPos-1); 
        quicksort(v, pPos+1, right);
    }

} // namespace adi_sorting