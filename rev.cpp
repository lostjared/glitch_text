#include<iostream>
#include<sstream>

int main() {
	std::ostringstream stream;
	stream << std::cin.rdbuf();
	std::string text = stream.str();
	for(auto s = text.rbegin(); s != text.rend(); ++s) {
		std::cout << *s;
	}
	return 0;
}

