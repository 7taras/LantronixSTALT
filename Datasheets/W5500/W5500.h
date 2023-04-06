// Date: 2023-04-06 
// Author: https://github.com/7taras/
// W5500 API's.

#pragma once
#include "W5500_Memory.h"

class W5500
{
public:
	W5500();
	~W5500();
	void reset();
	void readVersion();
};