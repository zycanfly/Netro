#include "MemManager.h"
#include<fstream>


u8& MemManager::access(u16 addr) {
	if (addr >= 0x0000 && addr < 0x2000) {
		return ram[addr % 0x800];
	}
	else if (addr > 0x4017) {
		return rom->access(addr);
	}
}

void MemManager::loadROM(const char* filename) {
	FILE* f = fopen(filename, "rb");

	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);

	ROM* rom = new ROM(new u8[size]);
	fread(rom->get(), size, 1, f);
	fclose(f);
}