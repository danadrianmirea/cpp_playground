#include "huffman_encoding.h"
#include <sstream>
#include <iostream>

HuffmanEncoding::HuffmanEncoding() : root(nullptr) {}

void HuffmanEncoding::buildFrequencyMap(const std::string& text, std::unordered_map<char, int>& freq_map) {
    for (char c : text) {
        freq_map[c]++;
    }
}

void HuffmanEncoding::buildHuffmanTree(const std::unordered_map<char, int>& freq_map) {
    // Create a priority queue to store nodes
    std::priority_queue<std::shared_ptr<HuffmanNode>, 
                       std::vector<std::shared_ptr<HuffmanNode>>, 
                       CompareNodes> pq;

    // Create leaf nodes for each character and add to priority queue
    for (const auto& pair : freq_map) {
        pq.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
    }

    // Build the Huffman tree
    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();

        auto parent = std::make_shared<HuffmanNode>('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    if (!pq.empty()) {
        root = pq.top();
    }
}

void HuffmanEncoding::generateCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code) {
    if (!node) return;

    // If leaf node, store the code
    if (!node->left && !node->right) {
        encoding_map[node->character] = code;
        decoding_map[code] = node->character;
        return;
    }

    // Traverse left and right
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

void HuffmanEncoding::clearMaps() {
    encoding_map.clear();
    decoding_map.clear();
    root = nullptr;
}

std::string HuffmanEncoding::encodeAsString(const std::string& text) {
    if (text.empty()) return "";

    // Clear previous encoding
    clearMaps();

    // Build frequency map
    std::unordered_map<char, int> freq_map;
    buildFrequencyMap(text, freq_map);

    // Build Huffman tree
    buildHuffmanTree(freq_map);

    // Generate codes
    generateCodes(root, "");

    // Encode the text
    std::string encoded;
    for (char c : text) {
        encoded += encoding_map[c];
    }

    return encoded;
}

std::string HuffmanEncoding::decode(const std::string& encoded_text) {
    if (encoded_text.empty() || !root) return "";

    std::string decoded;
    std::string current_code;
    
    // Traverse the tree for each bit
    for (char bit : encoded_text) {
        current_code += bit;
        
        // If we found a valid code, add the character and reset
        if (decoding_map.find(current_code) != decoding_map.end()) {
            decoded += decoding_map[current_code];
            current_code.clear();
        }
    }

    return decoded;
}

std::vector<uint8_t> HuffmanEncoding::packBits(const std::string& bitString) {
    std::vector<uint8_t> packed;
    uint8_t currentByte = 0;
    int bitCount = 0;

    for (char bit : bitString) {
        // Shift left and add the new bit
        currentByte = (currentByte << 1) | (bit - '0');
        bitCount++;

        // When we have 8 bits, add to the vector and reset
        if (bitCount == 8) {
            packed.push_back(currentByte);
            currentByte = 0;
            bitCount = 0;
        }
    }

    // Only add the last byte if there are remaining bits
    if (bitCount > 0) {
        currentByte <<= (8 - bitCount);
        packed.push_back(currentByte);
    }

    return packed;
}

std::string HuffmanEncoding::unpackBits(const std::vector<uint8_t>& packedData, size_t originalBitLength) {
    if (packedData.empty()) return "";

    std::string bitString;
    bitString.reserve(originalBitLength);  // Pre-allocate space for efficiency
    
    // Process each byte
    for (size_t i = 0; i < packedData.size(); i++) {
        uint8_t byte = packedData[i];
        // For the last byte, only process the bits up to originalBitLength
        int bitsToProcess = (i == packedData.size() - 1) ? 
            (originalBitLength % 8 == 0 ? 8 : originalBitLength % 8) : 8;
            
        for (int j = 7; j >= 8 - bitsToProcess; j--) {
            if (bitString.length() < originalBitLength) {
                bitString += ((byte >> j) & 1) ? '1' : '0';
            }
        }
    }

    return bitString;
}

void HuffmanEncoding::Test(const std::string& inputString) {
    std::string text = inputString;
    std::string encoded = encodeAsString(text);
    std::string decoded = decode(encoded);

    std::cout << "Original text: " << text << std::endl;
    std::cout << "Size of original text: " << text.length() << std::endl << std::endl;
    
    std::cout << "Encoded text: " << encoded << std::endl << std::endl;
    std::cout << "Decoded text: " << decoded << std::endl << std::endl;

    std::vector<uint8_t> packed = packBits(encoded);
    std::string unpacked = unpackBits(packed, encoded.length());

    std::cout << "Packed data: ";
    for (uint8_t byte : packed) {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << "Size of packed data: " << packed.size() << std::endl << std::endl;

    std::cout << "Unpacked data: " << unpacked << std::endl << std::endl;
    std::string decoded_from_packed = decode(unpacked);
    std::cout << "Decoded from packed data: " << decoded_from_packed << std::endl << std::endl;

    if (decoded_from_packed == text) {
        std::cout << "Decoded text matches original text" << std::endl;
    } else {
        std::cout << "Decoded text does not match original text" << std::endl;
    }
}