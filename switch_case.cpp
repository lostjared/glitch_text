#include<iostream>
#include<sstream>
#include<cstdlib>
#include<cctype>

std::string twistCase(bool start, std::string_view data) {
    std::string text;
    text.reserve(data.size());
    bool lead = start;
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (!lead)
            text += tolower(static_cast<unsigned char>(*it));
        else
            text += toupper(static_cast<unsigned char>(*it));
	if(isalpha(static_cast<unsigned char>(*it)))
	        lead = !lead;
    }
    return text;
}

int main(int argc, char **argv) {
	if(argc != 2) {
		std::cerr << "use: --upper/--lower" << std::endl;
		return EXIT_FAILURE;
	}
	std::string arg = argv[1];
	if(arg != "--upper" && arg != "--lower") {
		std::cerr << "use: --upper, or --lower" << std::endl;
		return EXIT_FAILURE;
	}
	bool start = arg == "--upper" ? true : false;
	std::ostringstream stream;
	stream << std::cin.rdbuf();
	std::cout << twistCase(start, stream.str());
	return EXIT_SUCCESS;
}

