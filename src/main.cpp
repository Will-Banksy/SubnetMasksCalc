#include <iostream>
#include "Utils.h"
#include <cmath>

#define CHECK_EXIT(x) if(x == "exit") exit(0)

int main() {
	std::cout << "Subnet Mask Calculator\n" << std::endl;

	std::cout << "Valid Powers of 2:\n";
	std::cout << "For Class A: " << Utils::ArrayToString(SubnetMask::s_ValidNumSubnets, 0, 24) << '\n';
	std::cout << "For Class B: " << Utils::ArrayToString(SubnetMask::s_ValidNumSubnets, 0, 16) << '\n';
	std::cout << "For Class C: " << Utils::ArrayToString(SubnetMask::s_ValidNumSubnets, 0, 8) << '\n' << std::endl;

	NetworkClass netClass = CLASS_A;
	uint64 numSubnets = 0;
	IPAddress* address = nullptr;

	Utils::GetInput("Enter the class of the network (A, B, or C) or an IP Address: ", [&netClass, &address](std::string str) {
		CHECK_EXIT(str);

		if(str == "A") {
			netClass = CLASS_A;
			return true;
		} else if(str == "B") {
			netClass = CLASS_B;
			return true;
		} else if(str == "C") {
			netClass = CLASS_C;
			return true;
		}

		std::vector<std::string> tokens = Utils::split(str, '.');
		if(tokens.size() != 4) {
			return false;
		}

		try {
			auto toUChar = [](int i) {
				if(i < 0) throw std::out_of_range("Out of range");
				else if(i > 255) throw std::out_of_range("Out of range");
				else return (uchar)i;
			};
			uchar b3 = toUChar(std::stoi(tokens.at(0)));
			uchar b2 = toUChar(std::stoi(tokens.at(1)));
			uchar b1 = toUChar(std::stoi(tokens.at(2)));
			uchar b0 = toUChar(std::stoi(tokens.at(3)));
			address = new IPAddress(b3, b2, b1, b0);
			netClass = address->GetNetClass();
			return true;
		} catch(std::invalid_argument except) { return false; }
		catch(std::out_of_range except) { return false; }

		return false;
	});

	InputMode mode = SUBNET_MASK;

	Utils::GetInput("Choose an input mode\n"
					"1: Enter a subnet mask\n"
					"2: Enter the (minimum) number of subnets you want\n"
					"3: Enter the (minimum) number of hosts you want\n"
					"4: Enter a CIDR number\n"
					"Enter input mode number: ", [&mode](std::string str) {
		CHECK_EXIT(str);

		if(str == "1") {
			mode = SUBNET_MASK;
			return true;
		} else if(str == "2") {
			mode = NUM_SUBNETS;
			return true;
		} else if(str == "3") {
			mode = NUM_HOSTS;
			return true;
		} else if(str == "4") {
			mode = CIDR_NUM;
			return true;
		}

		return false;
	});

	SubnetMask mask(netClass);
	uchar CIDR = 0;

	// TODO: Make sure that the number of possible subnets and the number of possible hosts (usable and non-usable) are both always displayed

	switch(mode) {
		case SUBNET_MASK: {
			// TODO
			break;
		}

		case NUM_SUBNETS: {
			Utils::GetInput("Enter the number of subnets you want: ", [&numSubnets, &mask, &CIDR](std::string str) {
				CHECK_EXIT(str);

				try {
					uint64 val = Utils::ParseUint(str);
					// TODO: Round to next power of 2
					for(int i = 0; i < 25; i++) {
						if(SubnetMask::s_ValidNumSubnets[i] >= val) {
							val = SubnetMask::s_ValidNumSubnets[i];
							break;
						}
					}
					std::cout << "Number of Subnets: " << val << std::endl;
					uchar suffixBits = 8 * (4 - (1 + mask.GetNetClass()));
					if(val > pow(2, suffixBits)) {
						throw std::invalid_argument("Too many subnets!");
					}
					numSubnets = val;
					CIDR = mask.Split(numSubnets);
					return true;
				} catch(std::invalid_argument except) {}
				catch(std::out_of_range except) {}
				return false;
			});
			break;
		}

		case NUM_HOSTS: {
			// TODO
			break;
		}

		case CIDR_NUM: {
			// TODO
			break;
		}
	}


	if(!address) {
		address = new IPAddress();
		*address = IPAddress::ExampleFor(netClass);
	}
	std::string ipaddress = address->ToString();

	std::cout << "Subnet Mask: " << mask.ToString() << std::endl;
	std::cout << "IP Address: " << ipaddress << "/" << (ushort)CIDR << std::endl;

	IPAddress start;
	IPAddress end;
	mask.NthRangeOf(0, *address, start, end);
	std::string firstRange = start.ToString() + " - " + end.ToString();
	mask.NthRangeOf(1, *address, start, end);
	std::string secondRange = start.ToString() + " - " + end.ToString();
	mask.NthRangeOf(numSubnets - 2, *address, start, end);
	std::string secondLastRange = start.ToString() + " - " + end.ToString();
	mask.NthRangeOf(numSubnets - 1, *address, start, end);
	std::string lastRange = start.ToString() + " - " + end.ToString();

	std::cout << "First Range: " << firstRange << std::endl;
	std::cout << "Second Range: " << secondRange << std::endl;
	std::cout << "Second Last Range: " << secondLastRange << std::endl;
	std::cout << "Second Last Range: " << lastRange << std::endl;

	std::cout << "\nPress Enter to exit...";
	std::cin.get();
    return 0;
}
