#include<iostream>
#include<string_view>
#include<string>
#include<ranges>
#include<random>
#include<algorithm>
#include<cstdlib>
#include<sstream>
#include<vector>

std::random_device rd;
std::mt19937 gen(rd());

std::string shuffle_word(std::string_view value) {
        std::string text;
        if(value.length() < 4)
                text = value;
        else {
                char c1 = value[0];
                char c2 = value.back();
                std::string v = std::string(value);
                std::string temp = v.substr(1, value.length()-1);
                std::ranges::shuffle(temp, gen);
                std::string word;
                word += c1;
                word += temp;
                word += c2;
                text = word;
        }
        return text;
}

std::vector<std::string> split_words(std::string_view words) {
    std::vector<std::string> v;
    std::string word;
    for (auto it = words.begin(); it != words.end(); ++it) {
        if (isalpha(static_cast<unsigned char>(*it))) {
            word += *it;
        } else {
            if (word.length() > 0) {
                v.push_back(shuffle_word(word));
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        v.push_back(shuffle_word(word));
        word.clear();
    }
    return v;
}

int main(int argc, char **argv) {
	std::ostringstream stream;
	stream << std::cin.rdbuf();
	std::vector<std::string> words = split_words(stream.str());
	for(const auto &i : words) {
		std::cout << i << " ";
	}
	return EXIT_SUCCESS;
}
