#include <iostream>
#include "Utils.h"
#include <cmath>
#include <bitset>

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

			std::cout << "Network Class: ";
			switch(address->GetFurtherNetClass()) {
				case CLASS_A_PUBLIC: std::cout << "Class A (Public)"; break;
				case CLASS_A_PRIVATE: std::cout << "Class A (Private)"; break;
				case CLASS_B_PUBLIC: std::cout << "Class B (Public)"; break;
				case CLASS_B_PRIVATE: std::cout << "Class B (Private)"; break;
				case CLASS_C_PUBLIC: std::cout << "Class C (Public)"; break;
				case CLASS_C_PRIVATE: std::cout << "Class C (Private)"; break;
				case CLASS_OTHER: std::cout << "Other"; break;
			}
			std::cout << std::endl;

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

	// Each case takes care of assigning the correct values to BOTH of numSubnets & CIDR - Each also needs to split mask
	// Each case also prints the number of subnets and number of hosts
	switch(mode) {
		case SUBNET_MASK: {
			Utils::GetInput("Enter a subnet mask: ", [&mask, &CIDR, &numSubnets](std::string str) {
				CHECK_EXIT(str);

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

					uint32 maskBytes = uint32(b3 << 24) | uint32(b2 << 16) | uint32(b1 << 8) | uint32(b0);
					std::bitset<32> bits(maskBytes);
					uchar counter = 0;
					for(int i = 31; i >= 0; i--) {
						if(!bits.test(i)) {
							CIDR = counter;
							break;
						}
						counter++;
					}

					mask = SubnetMask::FromCIDR(mask.GetNetClass(), CIDR);

					uchar numSuffixBits = 8 * (4 - (1 + mask.GetNetClass()));
					uint64 power = 32 - CIDR;

					std::cout << "Number of Hosts: " << (uint64)pow(2, power) << std::endl;
					numSubnets = pow(2, numSuffixBits - power);
					std::cout << "Number of Subnets: " << numSubnets << std::endl;

					return true;
				} catch(std::invalid_argument except) { return false; }
				catch(std::out_of_range except) { return false; }

				return false;
			});
			break;
		}

		case NUM_SUBNETS: {
			Utils::GetInput("Enter the number of subnets you want: ", [&numSubnets, &mask, &CIDR](std::string str) {
				CHECK_EXIT(str);

				try {
					uint64 val = Utils::ParseUint(str);
					for(int i = 0; i < 25; i++) {
						if(SubnetMask::s_ValidNumSubnets[i] >= val) {
							val = SubnetMask::s_ValidNumSubnets[i];
							break;
						}
					}
					uchar suffixBits = 8 * (4 - (1 + mask.GetNetClass()));
					int power = SubnetMask::GetPower(val);
					uint64 numHosts = pow(2, suffixBits - power); // Calculate number of hosts

					std::cout << "Number of Hosts: " << numHosts << std::endl;
					std::cout << "Number of Subnets: " << val << std::endl;
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
			Utils::GetInput("Enter the number of hosts you want: ", [&numSubnets, &mask, &CIDR](std::string str) {
				CHECK_EXIT(str);

				try {
					uint64 val = Utils::ParseUint(str);
					val += 2; // To account for the 2 unusable addresses
					for(int i = 0; i < 25; i++) {
						if(SubnetMask::s_ValidNumSubnets[i] >= val) {
							val = SubnetMask::s_ValidNumSubnets[i];
							break;
						}
					}
					std::cout << "Number of Hosts: " << val << std::endl;
					uchar suffixBits = 8 * (4 - (1 + mask.GetNetClass()));
					int power = SubnetMask::GetPower(val);
					val = pow(2, suffixBits - power); // Calculate the number of subnets

					std::cout << "Number of Subnets: " << val << std::endl;
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

		case CIDR_NUM: {
			Utils::GetInput("Enter CIDR Number: ", [&CIDR, &mask, &numSubnets](std::string str) {
				CHECK_EXIT(str);

				try {
					uint64 val = Utils::ParseUint(str);
					CIDR = val;

					mask = SubnetMask::FromCIDR(mask.GetNetClass(), CIDR);

					uchar numSuffixBits = 8 * (4 - (1 + mask.GetNetClass()));
					uint64 power = 32 - CIDR;

					std::cout << "Number of Hosts: " << (uint64)pow(2, power) << std::endl;
					numSubnets = pow(2, numSuffixBits - power);
					std::cout << "Number of Subnets: " << numSubnets << std::endl;
					return true;
				} catch(std::invalid_argument except) {}
				catch(std::out_of_range except) {}
				return false;
			});
			break;
		}
	}


	if(!address) {
		address = new IPAddress();
		*address = IPAddress::ExampleFor(netClass);
	}
	std::string ipaddress = address->ToString();

	std::cout << "Subnet Mask: " << mask.ToString() << std::endl;
	std::cout << "CIDR Notation: " << ipaddress << "/" << (ushort)CIDR << std::endl;

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

	std::cout << "\nFirst Range: " << firstRange << std::endl;
	std::cout << "Second Range: " << secondRange << std::endl;
	std::cout << "Second Last Range: " << secondLastRange << std::endl;
	std::cout << "Last Range: " << lastRange << '\n' << std::endl;

	Utils::GetInput("Enter 'n1:n2', where n1 is the first range to display and n2 is the last (Leave blank to skip): ", [&mask, &start, &end, &address, &numSubnets](std::string str) {
		if(str == "") {
			return true;
		}
		auto min = [](uint64 val1, uint64 val2) {
			if(val1 > val2) return val2;
					else return val1;
		};

		std::vector<std::string> tokens = Utils::split(str, ':');
		if(tokens.size() != 2) {
			return false;
		}

		try {
			uint64 startRange = Utils::ParseUint(tokens.at(0));
			uint64 endRange = Utils::ParseUint(tokens.at(1));

			std::cout << "Ranges " << startRange << " - " << endRange << " [" << std::endl;
			for(uint64 i = startRange; i < min(endRange, numSubnets - 1); i++) {
				mask.NthRangeOf(i, *address, start, end);
				std::cout << start.ToString() << " - " << end.ToString() << '\n';
			}
			std::cout << "]" << std::endl;
		} catch(std::invalid_argument except) {}
		catch(std::out_of_range except) {}

		return false;
	});

	std::cout << "\nPress Enter to exit...";
	std::cin.get();
    return 0;
}
