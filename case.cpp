#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <errno.h>
#include <iostream>
#include <random>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

std::string skipSpace(const std::string &text) {
    std::string output;
    std::size_t index = 0;
    std::size_t count = 0;
    while (index < text.length()) {
        if (text[index] == ' ' || text[index] == '\n') {
            if (count == 0) {
                output += ' ';
                count++;
            } else {
                count++;
            }
        } else {
            output += text[index];
            count = 0;
        }

        ++index;
    }

    return output;
}

std::string twistCase(bool start, std::string_view data) {
    std::string text;
    text.reserve(data.size());
    bool lead = start;
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (!lead)
            text += tolower(static_cast<unsigned char>(*it));
        else
            text += toupper(static_cast<unsigned char>(*it));
        lead = !lead;
    }
    return text;
}

void echo_text(std::string_view text, const std::size_t delayUs, const std::size_t chunkSize) {
    for (std::size_t i = 0; i < text.size(); i += chunkSize) {
        std::string_view slice = text.substr(i, chunkSize);
        std::cout.write(slice.data(), static_cast<std::streamsize>(slice.size()));
        std::cout.flush();
        if (delayUs > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(delayUs));
        }
    }
}

std::vector<std::string> split_words(std::string_view words) {
    std::vector<std::string> v;
    std::string word;
    for (auto it = words.begin(); it != words.end(); ++it) {
        if (isalpha(static_cast<unsigned char>(*it))) {
            word += *it;
        } else {
            if (word.length() > 0) {
                v.push_back(word);
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        v.push_back(word);
        word.clear();
    }
    return v;
}

std::string join_words(const std::vector<std::string> &v) {
    std::string text;
    for (const auto &i : v) {
        text += i + " ";
    }
    return text;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Use: " << argv[0] << " --upper delay" << std::endl;
        std::cerr << "Or: " << argv[0] << " --lower delay" << std::endl;
        return EXIT_FAILURE;
    }
    bool lead = false;
    std::string arg_val = argv[1];
    if (arg_val == "--upper")
        lead = true;
    else if (arg_val == "--lower")
        lead = false;
    else {
        std::cerr << "Error  invalid argument." << std::endl;
        return EXIT_FAILURE;
    }
    std::ostringstream stream;
    stream << std::cin.rdbuf();
    char *ptr = nullptr;
    errno = 0;
    long val = std::strtol(argv[2], &ptr, 10);
    if (errno == ERANGE || ptr == argv[2] || *ptr != '\0' || val < 0) {
        std::cerr << "Error converting to string from integer." << std::endl;
        return EXIT_FAILURE;
    }
    static char outBuffer[65536];
    std::cout.rdbuf()->pubsetbuf(outBuffer, sizeof(outBuffer));
    std::string value = skipSpace(stream.str());
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::string> words = split_words(value);
    if (words.empty())
        return EXIT_SUCCESS;
    while (1) {
        std::ranges::shuffle(words, gen);
        std::string word_code = join_words(words);
        std::string case_text = twistCase(lead, word_code);
        echo_text(case_text, val, 16);
        lead = !lead;
    }
    return EXIT_SUCCESS;
}
