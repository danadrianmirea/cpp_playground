#include <iostream>
#include "huffman_encoding.h"

int main()
{
    HuffmanEncoding huffman;

std::string text = 
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "The quick brown fox jumps over the lazy dog. This sentence contains every letter in the English alphabet. "
    "Huffman coding will compress this text more efficiently if some letters appear more frequently. Let's see!";



    huffman.Test(text);

    return 0;
}