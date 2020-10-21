#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <functional>
#include "SubnetMask.h"
#include <vector>

typedef std::function<bool(std::string)> InputChecker;

/**
 * @todo write docs
 */
class Utils {
private:
	Utils() = delete;

public:
	static SubnetMask ClassA;
	static SubnetMask ClassB;
	static SubnetMask ClassC;

public:
	static void GetInput(const std::string& message, const InputChecker& checker);
	static uint64 ParseUint(const std::string& str);
	/// Splits strings into tokens with delimiter <code>delim</code>.
	static std::vector<std::string> split(std::string str, char delim, bool ignoreEmpty = false);
	static std::string ArrayToString(const uint64* array, uint64 start, uint64 end);
};

#endif // UTILS_H
