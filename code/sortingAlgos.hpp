#pragma once

#include <iostream>
#include <vector>

namespace adi_sorting_algos
{
    inline void swap(int &a, int &b)
    {
        int temp = a;
        a = b;
        b = temp;
    }

    void bubble_sort(std::vector<int>& v)
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
} // namespace adi_sorting