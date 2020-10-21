#ifndef IPADDRESS_H
#define IPADDRESS_H

#include "Includes.h"
#include <string>

/**
 * @todo write docs
 */
class IPAddress {
public:
	IPAddress(uchar b3, uchar b2, uchar b1, uchar b0);
	IPAddress();

	static IPAddress ExampleFor(NetworkClass netClass);
	std::string ToString() const;
	NetworkClass GetNetClass() const { return m_NetClass; }
	void SetBytes(uchar b3, uchar b2, uchar b1, uchar b0);
	uchar GetB3() { return m_b3; }
	uchar GetB2() { return m_b2; }
	uchar GetB1() { return m_b1; }
	uchar GetB0() { return m_b0; }

protected:
	IPAddress(NetworkClass netClass);

	uchar m_b3;
	uchar m_b2;
	uchar m_b1;
	uchar m_b0;
	NetworkClass m_NetClass;
};

#endif // IPADDRESS_H
