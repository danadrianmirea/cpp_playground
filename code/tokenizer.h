#pragma once

#include <vector>
#include <string>


class Tokenizer
{
    public:
        std::vector<std::string> tokenize(const std::string& str, char delimiter);
        std::vector<std::string> tokenize_with_find(const std::string& str, char delimiter);
        void test();
};