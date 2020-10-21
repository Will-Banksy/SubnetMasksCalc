#include "SubnetMask.h"
#include <cmath>
#include <sstream>
#include <iostream>
#include <bitset>

const uint64* SubnetMask::s_ValidNumSubnets = GetValidNumSubnets();

SubnetMask::SubnetMask(NetworkClass netClass) : IPAddress(netClass), m_NumSubnets(0) {
	switch(netClass) {
		case CLASS_A:
			SetBytes(255, 0, 0, 0);
			break;
		case CLASS_B:
			SetBytes(255, 255, 0, 0);
			break;
		case CLASS_C:
			SetBytes(255, 255, 255, 0);
			break;
	}
}

uint64* SubnetMask::GetValidNumSubnets() {
	uint64* valids = new uint64[25];
	for(int i = 0; i < 25; i++) {
		valids[i] = (uint64)pow(2, i);
	}
	return valids;
}

uchar SubnetMask::Split(uint64 numSubnets) {
	int power = GetPower(numSubnets);
	if(power == -1 && numSubnets != 0) {
		throw std::invalid_argument("Invalid number of subnets");
	}

	m_NumSubnets = numSubnets;
// 	uchar suffixBits = 8 * (4 - (1 + m_NetClass)); // Can't use this as std::bitset needs a constant value as N
	switch(m_NetClass) {
		case CLASS_A: {
			if(numSubnets > pow(2, 24)) throw std::invalid_argument("Too many subnets!");
			std::bitset<24> bits;
			bool setBits = false;
			uchar at = 0;
			for(uchar i = 0; i < 24; i++) {
				if(pow(2, 24 - i) == numSubnets) {
					setBits = true;
					at = 24 - i;
				}
				if(setBits) {
					bits.set(i); // Sets the bit to 1
				}
			}
			m_b0 = (uchar)(bits.to_ulong() & 0xff);
			m_b1 = (uchar)((bits.to_ulong() >> 8) & 0xff);
			m_b2 = (uchar)((bits.to_ulong() >> 16) & 0xff);
			return at + 8;
		}

		case CLASS_B: {
			if(numSubnets > pow(2, 16)) throw std::invalid_argument("Too many subnets!");
			std::bitset<16> bits;
			bool setBits = false;
			uchar at = 0;
			for(uchar i = 0; i < 16; i++) {
				if(pow(2, 16 - i) == numSubnets) {
					setBits = true;
					at = 16 - i;
				}
				if(setBits) {
					bits.set(i); // Sets the bit to 1
				}
			}
			m_b0 = (uchar)(bits.to_ulong() & 0xff);
			m_b1 = (uchar)((bits.to_ulong() >> 8) & 0xff);
			return at + 16;
		}

		case CLASS_C: {
			if(numSubnets > pow(2, 8)) throw std::invalid_argument("Too many subnets!");
			std::bitset<8> bits;
			bool setBits = false;
			uchar at = 0;
			for(uchar i = 0; i < 8; i++) {
				if(pow(2, 8 - i) == numSubnets) {
					setBits = true;
					at = 8 - i;
				}
				if(setBits) {
					bits.set(i); // Sets the bit to 1
				}
			}
			m_b0 = (uchar)bits.to_ulong();
			return at + 24;
		}
	}
	return 0;
}

int SubnetMask::GetPower(uint64 numSubnets) {
	for(int i = 0; i < 25; i++) {
		if(numSubnets == s_ValidNumSubnets[i]) {
			return i;
		}
	}
	return -1;
}

void SubnetMask::NthRangeOf(uint64 n, IPAddress addr, IPAddress& startIP, IPAddress& endIP) {
	switch(addr.GetNetClass()) {
		case CLASS_A: {
			uint64 hostNum = 16777216;
			uint64 start = m_NumSubnets == 0 ? 0 : (int)(((float)hostNum / m_NumSubnets) * n);
			uint64 length = m_NumSubnets == 0 ? hostNum : (int)((float)hostNum / m_NumSubnets);
			uint64 end = start + length - 1;
			if(start > (hostNum - 1) | end > (hostNum - 1)) {
				throw std::logic_error("ARGGGGGGGFGGG!");
			}
			startIP.SetBytes(addr.GetB3(), (start >> 16) & 0xff, (start >> 8) & 0xff, start & 0xff);
			endIP.SetBytes(addr.GetB3(), (end >> 16) & 0xff, (end >> 8) & 0xff, end & 0xff);
// 			std::cout << "offset: " << start << ", n: " << n << ", numSubnets: " << m_NumSubnets << ", length: " << length << ", end: " << end << std::endl;
			break;
		}

		case CLASS_B: {
			uint64 hostNum = 65536;
			uint64 start = m_NumSubnets == 0 ? 0 : (int)(((float)hostNum / m_NumSubnets) * n);
			uint64 length = m_NumSubnets == 0 ? hostNum : (int)((float)hostNum / m_NumSubnets);
			uint64 end = start + length - 1;
			if(start > (hostNum - 1) | end > (hostNum - 1)) {
				throw std::logic_error("ARGGGGGGGFGGG!");
			}
			startIP.SetBytes(addr.GetB3(), addr.GetB2(), (start >> 8) & 0xff, start & 0xff);
			endIP.SetBytes(addr.GetB3(), addr.GetB2(), (end >> 8) & 0xff, end & 0xff);
			break;
		}

		case CLASS_C: {
			uint64 hostNum = 256;
			uint64 start = m_NumSubnets == 0 ? 0 : (int)(((float)hostNum / m_NumSubnets) * n);
			uint64 length = m_NumSubnets == 0 ? hostNum : (int)((float)hostNum / m_NumSubnets);
			uint64 end = start + length - 1;
			if(start > (hostNum - 1) | end > (hostNum - 1)) {
				throw std::logic_error("ARGGGGGGGFGGG!");
			}
			startIP.SetBytes(addr.GetB3(), addr.GetB2(), addr.GetB1(), start);
			endIP.SetBytes(addr.GetB3(), addr.GetB2(), addr.GetB1(), end);
			break;
		}
	}
}
