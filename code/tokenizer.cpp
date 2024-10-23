#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "tokenizer.h"
#include "log.h"
#include "example.h"
#include "datastructures.h"
#include "testclass.h"

using namespace std;

std::vector<std::string> Tokenizer::tokenize(const std::string& str, char delimiter) 
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::vector<std::string> Tokenizer::tokenize_with_find(const std::string& str, char delimiter) 
{
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = 0;
    
    while ((end = str.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    
    tokens.push_back(str.substr(start)); // Add the last token
    return tokens;
}

void Tokenizer::test()
{
  string str = "This is an example string";
  vector<string> tokens = tokenize(str,' ');
  
  for(const auto& t : tokens)
  {
    cout << t << endl;
  }

  str = "And here is another example string";
  tokens = tokenize_with_find(str, ' ');

  for(const auto& t : tokens)
  {
    cout << t << endl;
  }
}