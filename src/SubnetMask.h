#ifndef SUBNETMASK_H
#define SUBNETMASK_H

#include <string>
#include "IPAddress.h"
#include "Includes.h"

/**
 * @todo write docs
 */
class SubnetMask : public IPAddress {
public:
	/// Defines a subnet mask b3.b2.b1.b0
	SubnetMask(NetworkClass netClass);

	// Allocate on heap
	static const uint64* s_ValidNumSubnets;

	uint64 m_NumSubnets;

	/// Changes this Subnet Mask to one that fits as many subnets as specified, and returns how many bits are in the Network Prefix & Subnet Prefix combined (In CIDR notation, this is the number after the slash)
	uchar Split(uint64 numSubnets);

	/// Gets the nth range of host addresses and writes the start of the range into <code>start</code> and the end of the range into <code>end</code>
	void NthRangeOf(uint64 n, IPAddress addr, IPAddress& startIP, IPAddress& endIP);
	static int GetPower(uint64 numSubnets);
	static SubnetMask FromCIDR(NetworkClass netClass, uchar CIDR);

private:
	static uint64* GetValidNumSubnets();
	/// Returns positive integer n, where the argument is 2^n, or -1 otherwise
};

#endif // SUBNETMASK_H
