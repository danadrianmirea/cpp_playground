#ifndef HUFFMAN_ENCODING_H
#define HUFFMAN_ENCODING_H

#include <string>
#include <unordered_map>
#include <queue>
#include <memory>
#include <vector>
#include <cstdint>

// Node structure for Huffman tree
struct HuffmanNode {
    char character;
    int frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

// Comparison function for priority queue
struct CompareNodes {
    bool operator()(const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
        return a->frequency > b->frequency;
    }
};

class HuffmanEncoding {
public:
    HuffmanEncoding();
    
    // Encode text using Huffman coding
    std::string encodeAsString(const std::string& text);
    
    // Decode Huffman encoded text
    std::string decode(const std::string& encoded_text);

    // Pack bits into bytes and unpack bytes into bits
    std::vector<uint8_t> packBits(const std::string& bitString);
    std::string unpackBits(const std::vector<uint8_t>& packedData, size_t originalBitLength);

    void Test(const std::string& inputString);

private:
    std::shared_ptr<HuffmanNode> root;
    std::unordered_map<char, std::string> encoding_map;
    std::unordered_map<std::string, char> decoding_map;

    // Helper functions
    void buildFrequencyMap(const std::string& text, std::unordered_map<char, int>& freq_map);
    void buildHuffmanTree(const std::unordered_map<char, int>& freq_map);
    void generateCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code);
    void clearMaps();
};

#endif // HUFFMAN_ENCODING_H 