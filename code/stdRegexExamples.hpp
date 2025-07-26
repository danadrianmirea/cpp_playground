#ifndef STD_REGEX_EXAMPLES_HPP
#define STD_REGEX_EXAMPLES_HPP

#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace regex_examples {

    void demoStdRegexExamples() {
        std::cout << "=== C++11 std::regex Examples Demo ===" << std::endl;

        // 1. Simple match
        std::string text = "The quick brown fox jumps over the lazy dog 123.";
        std::regex word_regex("[a-zA-Z]+", std::regex_constants::ECMAScript);
        std::smatch match;
        if (std::regex_search(text, match, word_regex)) {
            std::cout << "First word match: " << match.str() << std::endl;
        }

        // 2. Find all matches (words)
        std::cout << "All words: ";
        auto words_begin = std::sregex_iterator(text.begin(), text.end(), word_regex);
        auto words_end = std::sregex_iterator();
        for (auto it = words_begin; it != words_end; ++it) {
            std::cout << it->str() << " ";
        }
        std::cout << std::endl;

        // 3. Regex replace
        std::string replaced = std::regex_replace(text, std::regex("[0-9]+"), "<number>");
        std::cout << "Replaced numbers: " << replaced << std::endl;

        // 4. Email validation
        std::string email = "user@example.com";
        std::regex email_regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
        std::cout << "Email '" << email << "' is ";
        if (std::regex_match(email, email_regex)) {
            std::cout << "valid";
        } else {
            std::cout << "invalid";
        }
        std::cout << std::endl;

        // 5. Extract numbers
        std::regex number_regex("[0-9]+");
        std::cout << "Numbers in text: ";
        for (std::sregex_iterator it(text.begin(), text.end(), number_regex), end; it != end; ++it) {
            std::cout << it->str() << " ";
        }
        std::cout << std::endl;

        // 6. Split string by whitespace
        std::regex ws_re("\\s+");
        std::sregex_token_iterator token_it(text.begin(), text.end(), ws_re, -1);
        std::sregex_token_iterator token_end;
        std::cout << "Split by whitespace: ";
        for (; token_it != token_end; ++token_it) {
            std::cout << '"' << token_it->str() << '"' << " ";
        }
        std::cout << std::endl;

        // 7. Validate phone number
        std::string phone = "+1-800-555-1234";
        std::regex phone_regex(R"(\+\d{1,2}-\d{3}-\d{3}-\d{4})");
        std::cout << "Phone '" << phone << "' is ";
        if (std::regex_match(phone, phone_regex)) {
            std::cout << "valid";
        } else {
            std::cout << "invalid";
        }
        std::cout << std::endl;

        // 8. Greedy vs non-greedy
        std::string html = "<div>content</div><div>more</div>";
        std::regex greedy_re("<div>.*</div>");
        std::regex nongreedy_re("<div>.*?</div>");
        std::smatch greedy_match, nongreedy_match;
        std::regex_search(html, greedy_match, greedy_re);
        std::regex_search(html, nongreedy_match, nongreedy_re);
        std::cout << "Greedy match: " << greedy_match.str() << std::endl;
        std::cout << "Non-greedy match: " << nongreedy_match.str() << std::endl;

        // 9. Case-insensitive search
        std::regex case_insensitive_re("fox", std::regex_constants::icase);
        if (std::regex_search(text, match, case_insensitive_re)) {
            std::cout << "Case-insensitive match: " << match.str() << std::endl;
        }

        // 10. URL validation
        std::string url = "https://www.example.com/page?query=1";
        std::regex url_regex(R"((http|https)://(\w+\.)+(\w+)(/\S*)?)");
        std::cout << "URL '" << url << "' is ";
        if (std::regex_match(url, url_regex)) {
            std::cout << "valid";
        } else {
            std::cout << "invalid";
        }
        std::cout << std::endl;

        std::cout << "=== End of std::regex Demo ===" << std::endl;
    }

} // namespace regex_examples

#endif // STD_REGEX_EXAMPLES_HPP 