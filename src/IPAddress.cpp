#include "IPAddress.h"
#include <iostream>
#include <sstream>

IPAddress::IPAddress(uchar b3, uchar b2, uchar b1, uchar b0) : m_b3(b3), m_b2(b2), m_b1(b1), m_b0(b0) {
	if(m_b3 > 0 && m_b3 < 127) {
		m_NetClass = CLASS_A;
	}
	else if(m_b3 > 127 && m_b3 < 192) {
		m_NetClass = CLASS_B;
	}
	else if(m_b3 > 191 && m_b3 < 224) {
		m_NetClass = CLASS_C;
	}
	else throw std::invalid_argument("IP Address does not fit into any of the standard network classes");
}

IPAddress::IPAddress() : m_b3(0), m_b2(0), m_b1(0), m_b0(0), m_NetClass(CLASS_A) {
}


IPAddress::IPAddress(NetworkClass netClass) : m_NetClass(netClass) {
}

IPAddress IPAddress::ExampleFor(NetworkClass netClass) {
	switch(netClass) {
		case CLASS_A:
			return IPAddress(10, 0, 0, 0);

		case CLASS_B:
			return IPAddress(172, 16, 0, 0);

		case CLASS_C:
			return IPAddress(192, 168, 0, 0);
	}
	return IPAddress(0, 0, 0, 0);
}

std::string IPAddress::ToString() const {
	std::stringstream ss;
	ss << (ushort)m_b3 << "." << (ushort)m_b2 << "." << (ushort)m_b1 << "." << (ushort)m_b0;
	return ss.str();
}

void IPAddress::SetBytes(uchar b3, uchar b2, uchar b1, uchar b0) {
	m_b3 = b3;
	m_b2 = b2;
	m_b1 = b1;
	m_b0 = b0;
}