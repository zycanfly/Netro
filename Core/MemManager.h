#pragma once
#include"type.h"
class MemManager {
public:
	u8& access(u16 addr);
private:
	u8 ram[800];
};

