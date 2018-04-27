#pragma once
#include "type.h"
#include "MemManager.h"
enum IntType { NMI, RESET, IRQ, BRK };
enum FLAG { C, Z, I, D, V, N };

class Flag {
public:
	u8 get() { return f; }
	bool get(FLAG p) { return f >> p & 0x01; }
	void set(u8 p) { f = p; }
	void set(FLAG p, bool f) {
		if (f) f | 0x01 << p;
		else f&(0xFF - (0x01 << p));
	}
private:
	u8 f;
};
class CPU{
public:
	typedef u16(CPU::*Mode)(void);
	CPU(MemManager* mem) :memManager(mem) {};
	u8 getFlag() {return flag.get();}
	bool getFlag(FLAG f) { return flag.get() >> f & 0x01; }
	void exec();
private:
	int remainingCycles;
	Flag flag;
	u8 A, X, Y, S;
	u16 PC;
	bool nmi;
	MemManager* memManager;
#define T   tick()
	inline void tick() { remainingCycles--; }
	inline void upd_cv(u8 x, u8 y, s16 r) {flag.set(C, r > 0xFF); flag.set(V, ~(x^y) & (x^r) & 0x80);}
	inline void upd_nz(u8 x) { flag.set(N,x & 0x80); flag.set(Z,x == 0); }
	inline bool cross(u16 a, u8 i) { return ((a + i) & 0xFF00) != ((a & 0xFF00)); }

	inline u8 wr(u16 a, u8 v) { T; memManager->access(a) = v; return v; }
	inline u8 rd(u16 a) { T; return memManager->access(a); }

	inline u16 rd16(u16 a) { return memManager->access(a) | (memManager->access(a + 1)<<8); }
	inline u16 rd16_d(u16 a, u16 b) { return memManager->access(a) | (memManager->access(b) << 8); }
	inline u8 push(u8 v) { return wr(0x100 + (S--), v); }
	inline u8 pop() { return rd(0x100 + (++S)); }
	void dma_oam(u8 bank) { for (int i = 0; i < 256; i++)  wr(0x2014, rd(bank * 0x100 + i)<<8); }

	inline u16 imm()   { return PC++;                                       }
	inline u16 imm16() { PC += 2; return PC - 2;                            }
	inline u16 abs()   { return rd16(imm16());                              }
	inline u16 _abx()  { T; return abs() + X;                               }
	inline u16 abx()   { u16 a = abs(); if (cross(a, X)) T; return a + X;   }
	inline u16 aby()   { u16 a = abs(); if (cross(a, Y)) T; return a + Y;   }
	inline u16 zp()    { return rd(imm());                                  }
	inline u16 zpx()   { T; return (zp() + X) % 0x100;                      }
	inline u16 zpy()   { T; return (zp() + Y) % 0x100;                      }
	inline u16 izx()   { u8 i = zpx(); return rd16_d(i, (i+1) % 0x100);     }
	inline u16 _izy()  { u8 i = zp();  return rd16_d(i, (i+1) % 0x100) + Y; }
	inline u16 izy()   { u16 a = _izy(); if (cross(a-Y, Y)) T; return a;    }
#define G  u16 a = (this->*m)(); u8 p = rd(a)
#define LD(REG) template<Mode m> void ld##REG(){G;upd_nz(REG=p);}
	LD(A)LD(X)LD(Y)
#undef LD
#define CMP(REG) template<Mode m> void cmp##REG(){G,upd_nz(REG-p);flag.set(C,REG>=p);}
	CMP(A)CMP(X)CMP(Y)
#undef CMP
	template<Mode m> void ADC() { G; s16 r = A + p + flag.get(C); upd_cv(A, p, r); upd_nz(A = r); }
	template<Mode m> void SBC() { G ^ 0xFF; s16 r = A + p + flag.get(C); upd_cv(A, p, r); upd_nz(A = r); }
	template<Mode m> void BIT() { G; flag.set(Z ,A & p); flag.set(N,p & 0x80); flag.set(V,p & 0x40); }
	template<Mode m> void AND() { G; upd_nz(A &= p); }
	template<Mode m> void EOR() { G; upd_nz(A ^= p); }
	template<Mode m> void ORA() { G; upd_nz(A |= p); }
	template<Mode m> void ASL() { G; flag.set(C,p & 0x80); T; upd_nz(wr(a, p << 1)); }
	template<Mode m> void LSR() { G; flag.set(C,p & 0x01); T; upd_nz(wr(a, p >> 1)); }
	template<Mode m> void ROL() { G; u8 c = flag.get(C); flag.set(C,p & 0x80); T; upd_nz(wr(a, (p << 1) | c)); }
	template<Mode m> void ROR() { G; u8 c = flag.get(C) << 7; flag.set(C,p & 0x01); T; upd_nz(wr(a, c | (p >> 1))); }
	template<Mode m> void DEC() { G; T; upd_nz(wr(a, --p)); }
	template<Mode m> void INC() { G; T; upd_nz(wr(a, ++p)); }
#undef G
#define CHG(CHG,REG) void CHG##REG(){upd_nz(--REG); T; }
	CHG(dec,X)CHG(dec,Y)CHG(inc,X)CHG(inc,Y)
#undef CHG
	void ASL_A() { flag.set(C,A & 0x80); upd_nz(A <<= 1); T; }
	void LSR_A() { flag.set(C,A & 0x01); upd_nz(A >>= 1); T; }
	void ROL_A() { u8 c = flag.get(C); flag.set(C, A & 0x80); upd_nz(A = ((A << 1) | c)); T; }
	void ROR_A() { u8 c = flag.get(C) << 7; flag.set(C, A & 0x01); upd_nz(A = (c | (A >> 1))); T; }
#define TR(REG1,REG2) void tr##REG1##REG2(){upd_nz(REG1=REG2);T;}
	TR(X,A)TR(Y,A)TR(A,Y)TR(A,X)TR(S,X)
	void trXS() { S = X;T; }
	void PLP() { T; T; flag.set(pop()); }
	void PHP() { T; push(flag.get() | (1 << 4)); }
	void PLA() { T; T; A = pop(); upd_nz(A); }
	void PHA() { T; push(A); }
#undef TR
#define ST(REG) template<Mode m> void st##REG(){wr((this->*m)(),REG);}
	ST(A)ST(X)ST(Y)
#undef ST
	template<FLAG f, bool v> void br() { s8 j = rd(imm()); if (flag.get(f) == v) { T; PC += j; } }
	void JMP_IND() { u16 i = rd16(imm16()); PC = rd16_d(i, (i & 0xFF00) | ((i + 1) % 0x100)); }
	void JMP() { PC = rd16(imm16()); }
	void JSR() { u16 t = PC + 1; T; push(t >> 8); push(t); PC = rd16(imm16()); }
	void RTS() { T; T;  PC = (pop() | (pop() << 8)) + 1; T; }
	void RTI() { PLP(); PC = pop() | (pop() << 8); }
	template<FLAG f, bool v> void flags() { flag.set(f,v); T; }
	template<IntType t> void INT() {
		T; if (t != BRK) T;
		if (t != RESET)
		{
			push(PC >> 8); push(PC & 0xFF);
			push(flag.get() | ((t == BRK) << 4));
		}
		else { S -= 3; T; T; T; }
		flag.set(I, true);
		constexpr u16 vect[] = { 0xFFFA, 0xFFFC, 0xFFFE, 0xFFFE };
		PC = rd16(vect[t]);
		if (t == NMI) nmi = false;
	}
	void NOP() { T; }
};

