#pragma once
#include"type.h"
class ROM {
public:
	u8& access(u16 addr){
		if (addr >= 0x8000)
			return prg[prgMap[(addr - 0x8000) / 0x2000] + ((addr - 0x8000) % 0x2000)];
		//else
			//return prgRam[addr - 0x6000];
	}

	ROM(u8* _rom):rom(_rom){
		prgSize = rom[4] * 0x4000;
		chrSize = rom[5] * 0x2000;
		prgRamSize = rom[8] ? rom[8] * 0x2000 : 0x2000;
		//set_mirroring((rom[6] & 1) ? PPU::VERTICAL : PPU::HORIZONTAL);

		this->prg = rom + 16;
		this->prgRam = new u8[prgRamSize];
		if (chrSize)
			this->chr = rom + 16 + prgSize;
		else {
			chrRam = true;
			chrSize = 0x2000;
			this->chr = new u8[chrSize];
		}

		for (int i = 0; i < (32 / 8); i++)
			prgMap[i] = 0x2000 * i % prgSize;
		for (int i = 0; i < 8; i++)
			chrMap[i] = 0x400 * i % chrSize;
	}

	u8* get() { return rom; }
private:
	u8 * rom,*prg,*chr,*prgRam;
	u32 prgSize, chrSize, prgRamSize;
	u32 prgMap[4];
	u32 chrMap[8];
	bool chrRam = false;
};

