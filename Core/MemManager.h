#pragma once
#include"type.h"
#include"ROM.h"
class MemManager {
public:
	u8& access(u16 addr);
	void loadROM(const char* filename);

private:
	u8 ram[800];
	ROM* rom;
};

