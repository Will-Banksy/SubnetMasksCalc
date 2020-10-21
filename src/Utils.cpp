#include "Utils.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

SubnetMask Utils::ClassA = SubnetMask(CLASS_A);
SubnetMask Utils::ClassB = SubnetMask(CLASS_B);
SubnetMask Utils::ClassC = SubnetMask(CLASS_C);

void Utils::GetInput(const std::string& message, const InputChecker& checker) {
	while(true) {
		std::cout << message;
		std::cout.flush();
		std::string line;
		getline(std::cin, line);
		if(checker(line)) {
			break;
		}
	}
}

uint64 Utils::ParseUint(const std::string& str) {
	unsigned long lresult = stoul(str, 0, 10);
	uint64 result = lresult;
	if (result != lresult) throw std::out_of_range("Out of range");
	return result;
}

std::vector<std::string> Utils::split(std::string str, char delim, bool ignoreEmpty) {
	// Current token
	std::stringstream token;
	std::vector<std::string> tokens;

	for(int i = 0; i < str.length(); i++) {
		if(str.at(i) == delim) {
			token.flush();
			if(!ignoreEmpty || !token.str().empty()) {
				tokens.push_back(token.str());
			}
			token.str(std::string()); // Empty the current token
		} else {
			token << char(str.at(i)); // Should probably make sure it's copied
		}
	}
	token.flush();
	if(!ignoreEmpty || !token.str().empty()) {
		tokens.push_back(token.str());
	}

	return tokens;
}

std::string Utils::ArrayToString(const uint64* array, uint64 start, uint64 end) {
	std::stringstream ss;
	ss << "[";
	for(int i = start; i <= end; i++) {
		ss << array[i];
		if(i != end) {
			ss << ", ";
		}
	}
	ss << "]";
	ss.flush();
	return ss.str();
}