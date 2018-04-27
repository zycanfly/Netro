#include "MemManager.h"

u8& MemManager::access(u16 addr) {
	if (addr >= 0x0000 && addr < 0x2000) {
		return ram[addr % 0x800];
	}
}