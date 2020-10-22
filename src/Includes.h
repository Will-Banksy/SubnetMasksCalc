#ifndef INCLUDES_H
#define INCLUDES_H

#include <cstdlib>

typedef u_int8_t uchar;
typedef u_int32_t uint32;
typedef u_int64_t uint64;

enum NetworkClass {
	CLASS_A,
	CLASS_B,
	CLASS_C
};

enum InputMode {
	SUBNET_MASK,
	NUM_SUBNETS,
	NUM_HOSTS,
	CIDR_NUM
};

enum FurtherNetworkClass {
	CLASS_A_PUBLIC,
	CLASS_A_PRIVATE,
	CLASS_B_PUBLIC,
	CLASS_B_PRIVATE,
	CLASS_C_PUBLIC,
	CLASS_C_PRIVATE,
	CLASS_OTHER
};

#endif // INCLUDES_H