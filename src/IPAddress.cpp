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

FurtherNetworkClass IPAddress::GetFurtherNetClass() const {
	if(m_b3 > 0 && m_b3 < 127) {
		// Class A
		if(m_b3 == 10) {
			return CLASS_A_PRIVATE;
		} else {
			return CLASS_A_PUBLIC;
		}
	} else if(m_b3 > 127 && m_b3 < 192) {
		// Class B
		if((m_b3 > 127 && m_b3 < 172) || (m_b3 > 172 && m_b3 < 192)) {
			return CLASS_B_PUBLIC;
		} else { // Implied if(m_b3 == 172)
			if(m_b2 > 15 && m_b2 < 32) {
				return CLASS_B_PRIVATE;
			}
			return CLASS_B_PUBLIC;
		}
	} else if(m_b3 > 191 && m_b3 < 224) {
		// Class C
		if(m_b3 == 192) {
			if(m_b2 == 168) {
				return CLASS_C_PRIVATE;
			} else {
				return CLASS_C_PUBLIC;
			}
		} else {
			return CLASS_C_PUBLIC;
		}
	} else {
		return CLASS_OTHER;
	}
}
