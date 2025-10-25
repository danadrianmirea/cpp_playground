#pragma once

#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>

template<typename T>
class MaxHeap {
private:
    std::vector<T> heap;

    // Helper functions for heap operations
    int getParent(int index) const {
        return (index - 1) / 2;
    }

    int getLeftChild(int index) const {
        return 2 * index + 1;
    }

    int getRightChild(int index) const {
        return 2 * index + 2;
    }

    void heapifyUp(int index) {
        if (index == 0) return;
        
        int parentIndex = getParent(index);
        if (heap[index] > heap[parentIndex]) {
            std::swap(heap[index], heap[parentIndex]);
            heapifyUp(parentIndex);
        }
    }

    void heapifyDown(int index) {
        int leftChild = getLeftChild(index);
        int rightChild = getRightChild(index);
        int largest = index;

        if (leftChild < heap.size() && heap[leftChild] > heap[largest]) {
            largest = leftChild;
        }

        if (rightChild < heap.size() && heap[rightChild] > heap[largest]) {
            largest = rightChild;
        }

        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

public:
    // Constructor
    MaxHeap() = default;

    // Constructor from vector
    explicit MaxHeap(const std::vector<T>& elements) : heap(elements) {
        // Build heap from bottom up
        for (int i = (heap.size() / 2) - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    // Insert element into heap
    void insert(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    // Extract maximum element
    T extractMax() {
        if (isEmpty()) {
            throw std::runtime_error("Heap is empty");
        }

        T maxValue = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        
        if (!isEmpty()) {
            heapifyDown(0);
        }
        
        return maxValue;
    }

    // Peek at maximum element without removing
    const T& peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }

    // Get size of heap
    size_t size() const {
        return heap.size();
    }

    // Check if heap is empty
    bool isEmpty() const {
        return heap.empty();
    }

    // Clear the heap
    void clear() {
        heap.clear();
    }

    // Get all elements (for debugging/display purposes)
    std::vector<T> getElements() const {
        return heap;
    }

    // Print heap (for debugging)
    void printHeap() const {
        std::cout << "Heap: ";
        for (const auto& element : heap) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
};

// Demo function showcasing max heap functionality
inline void maxHeapDemo() {
    std::cout << "\n=== Max Heap Demo ===\n" << std::endl;

    // 1. Basic operations
    std::cout << "1. Basic Max Heap Operations:" << std::endl;
    MaxHeap<int> heap;
    
    // Insert elements
    std::vector<int> elements = {10, 20, 15, 30, 40, 25, 35};
    std::cout << "Inserting elements: ";
    for (int elem : elements) {
        std::cout << elem << " ";
        heap.insert(elem);
    }
    std::cout << std::endl;
    
    std::cout << "Heap size: " << heap.size() << std::endl;
    std::cout << "Current max element: " << heap.peek() << std::endl;
    heap.printHeap();
    std::cout << std::endl;

    // 2. Extract max elements
    std::cout << "2. Extracting max elements (heap sort):" << std::endl;
    std::cout << "Extracted in descending order: ";
    while (!heap.isEmpty()) {
        std::cout << heap.extractMax() << " ";
    }
    std::cout << std::endl << std::endl;

    // 3. Heap from vector constructor
    std::cout << "3. Building heap from vector:" << std::endl;
    std::vector<int> data = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::cout << "Original vector: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    MaxHeap<int> heapFromVector(data);
    std::cout << "Heap built from vector: ";
    heapFromVector.printHeap();
    std::cout << std::endl;

    // 4. Priority queue simulation
    std::cout << "4. Priority Queue Simulation:" << std::endl;
    MaxHeap<int> priorityQueue;
    
    // Simulate task priorities (higher number = higher priority)
    std::vector<std::pair<int, std::string>> tasks = {
        {3, "Low priority task"},
        {7, "High priority task"},
        {1, "Very low priority task"},
        {9, "Critical task"},
        {5, "Medium priority task"}
    };

    std::cout << "Adding tasks with priorities:" << std::endl;
    for (const auto& task : tasks) {
        std::cout << "  Priority " << task.first << ": " << task.second << std::endl;
        priorityQueue.insert(task.first);
    }

    std::cout << "\nProcessing tasks in priority order:" << std::endl;
    while (!priorityQueue.isEmpty()) {
        int priority = priorityQueue.extractMax();
        // Find the corresponding task description
        for (const auto& task : tasks) {
            if (task.first == priority) {
                std::cout << "  Processing: " << task.second << " (Priority: " << priority << ")" << std::endl;
                break;
            }
        }
    }
    std::cout << std::endl;

    // 5. Finding top K elements
    std::cout << "5. Finding top 3 largest elements:" << std::endl;
    std::vector<int> numbers = {15, 8, 22, 3, 17, 9, 25, 1, 19, 12, 6, 30, 4};
    std::cout << "Original numbers: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    MaxHeap<int> topKHeap;
    for (int num : numbers) {
        topKHeap.insert(num);
    }

    std::cout << "Top 3 largest elements: ";
    for (int i = 0; i < 3 && !topKHeap.isEmpty(); i++) {
        std::cout << topKHeap.extractMax() << " ";
    }
    std::cout << std::endl << std::endl;

    // 6. String heap demo
    std::cout << "6. Max Heap with Strings (lexicographic order):" << std::endl;
    MaxHeap<std::string> stringHeap;
    std::vector<std::string> words = {"apple", "banana", "cherry", "date", "elderberry"};
    
    std::cout << "Inserting words: ";
    for (const std::string& word : words) {
        std::cout << word << " ";
        stringHeap.insert(word);
    }
    std::cout << std::endl;

    std::cout << "Extracting in lexicographic order: ";
    while (!stringHeap.isEmpty()) {
        std::cout << stringHeap.extractMax() << " ";
    }
    std::cout << std::endl;

    std::cout << "\n=== Max Heap Demo Complete ===\n" << std::endl;
}
