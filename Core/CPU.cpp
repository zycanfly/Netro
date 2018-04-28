#include "CPU.h"
#include<iostream>
void CPU::exec() {
	switch (rd(PC++))
	{
	case 0x00: return INT<BRK>();			case 0x01: return ORA<&CPU::izx>();
	case 0x05: return ORA<&CPU::zp>();		case 0x06: return ASL<&CPU::zp>();
	case 0x08: return PHP();				case 0x09: return ORA<&CPU::imm>();
	case 0x0A: return ASL_A();				case 0x0D: return ORA<&CPU::abs>();
	case 0x0E: return ASL<&CPU::abs>();		case 0x10: return br<N, 0>();
	case 0x11: return ORA<&CPU::izy>();		case 0x15: return ORA<&CPU::zpx>();
	case 0x16: return ASL<&CPU::zpx>();		case 0x18: return flags<C, 0>();
	case 0x19: return ORA<&CPU::aby>();		case 0x1D: return ORA<&CPU::abx>();
	case 0x1E: return ASL<&CPU::_abx>();	case 0x20: return JSR();
	case 0x21: return AND<&CPU::izx>();		case 0x24: return BIT<&CPU::zp>();
	case 0x25: return AND<&CPU::zp>();		case 0x26: return ROL<&CPU::zp>();
	case 0x28: return PLP();				case 0x29: return AND<&CPU::imm>();
	case 0x2A: return ROL_A();				case 0x2C: return BIT<&CPU::abs>();
	case 0x2D: return AND<&CPU::abs>();		case 0x2E: return ROL<&CPU::abs>();
	case 0x30: return br<N, 1>();			case 0x31: return AND<&CPU::izy>();
	case 0x35: return AND<&CPU::zpx>();		case 0x36: return ROL<&CPU::zpx>();
	case 0x38: return flags<C, 1>();		case 0x39: return AND<&CPU::aby>();
	case 0x3D: return AND<&CPU::abx>();		case 0x3E: return ROL<&CPU::_abx>();
	case 0x40: return RTI();				case 0x41: return EOR<&CPU::izx>();
	case 0x45: return EOR<&CPU::zp>();		case 0x46: return LSR<&CPU::zp>();
	case 0x48: return PHA();				case 0x49: return EOR<&CPU::imm>();
	case 0x4A: return LSR_A();				case 0x4C: return JMP();
	case 0x4D: return EOR<&CPU::abs>();		case 0x4E: return LSR<&CPU::abs>();
	case 0x50: return br<V, 0>();			case 0x51: return EOR<&CPU::izy>();
	case 0x55: return EOR<&CPU::zpx>();		case 0x56: return LSR<&CPU::zpx>();
	case 0x58: return flags<I, 0>();		case 0x59: return EOR<&CPU::aby>();
	case 0x5D: return EOR<&CPU::abx>();		case 0x5E: return LSR<&CPU::_abx>();
	case 0x60: return RTS();				case 0x61: return ADC<&CPU::izx>();
	case 0x65: return ADC<&CPU::zp>();		case 0x66: return ROR<&CPU::zp>();
	case 0x68: return PLA();				case 0x69: return ADC<&CPU::imm>();
	case 0x6A: return ROR_A();				case 0x6C: return JMP_IND();
	case 0x6D: return ADC<&CPU::abs>();		case 0x6E: return ROR<&CPU::abs>();
	case 0x70: return br<V, 1>();			case 0x71: return ADC<&CPU::izy>();
	case 0x75: return ADC<&CPU::zpx>();		case 0x76: return ROR<&CPU::zpx>();
	case 0x78: return flags<I, 1>();		case 0x79: return ADC<&CPU::aby>();
	case 0x7D: return ADC<&CPU::abx>();		case 0x7E: return ROR<&CPU::_abx>();
	case 0x81: return stA<&CPU::izx>();		case 0x84: return stY<&CPU::zp>();
	case 0x85: return stA<&CPU::zp>();		case 0x86: return stX<&CPU::zp>();
	case 0x88: return decY();				case 0x8A: return trXA();
	case 0x8C: return stY<&CPU::abs>();		case 0x8D: return stA<&CPU::abs>();
	case 0x8E: return stX<&CPU::abs>();		case 0x90: return br<C, 0>();
	case 0x91: return stA<&CPU::izy>();		case 0x94: return stY<&CPU::zpx>();
	case 0x95: return stA<&CPU::zpx>();		case 0x96: return stX<&CPU::zpy>();
	case 0x98: return trYA();				case 0x99: return stA<&CPU::aby>();
	case 0x9A: return trXS();				case 0x9D: return stA<&CPU::abx>();
	case 0xA0: return ldY<&CPU::imm>();		case 0xA1: return ldA<&CPU::izx>();
	case 0xA2: return ldX<&CPU::imm>();		case 0xA4: return ldY<&CPU::zp>();
	case 0xA5: return ldA<&CPU::zp>();		case 0xA6: return ldX<&CPU::zp>();
	case 0xA8: return trAY();				case 0xA9: return ldA<&CPU::imm>();
	case 0xAA: return trAX();				case 0xAC: return ldY<&CPU::abs>();
	case 0xAD: return ldA<&CPU::abs>();		case 0xAE: return ldX<&CPU::abs>();
	case 0xB0: return br<C, 1>();			case 0xB1: return ldA<&CPU::izy>();
	case 0xB4: return ldY<&CPU::zpx>();		case 0xB5: return ldA<&CPU::zpx>();
	case 0xB6: return ldX<&CPU::zpy>();		case 0xB8: return flags<V, 0>();
	case 0xB9: return ldA<&CPU::aby>();		case 0xBA: return trSX();
	case 0xBC: return ldY<&CPU::abx>();		case 0xBD: return ldA<&CPU::abx>();
	case 0xBE: return ldX<&CPU::aby>();		case 0xC0: return cmpY<&CPU::imm>();
	case 0xC1: return cmpA<&CPU::izx>();	case 0xC4: return cmpY<&CPU::zp>();
	case 0xC5: return cmpA<&CPU::zp>();		case 0xC6: return DEC<&CPU::zp>();
	case 0xC8: return incY();				case 0xC9: return cmpA<&CPU::imm>();
	case 0xCA: return decX();				case 0xCC: return cmpY<&CPU::abs>();
	case 0xCD: return cmpA<&CPU::abs>();	case 0xCE: return DEC<&CPU::abs>();
	case 0xD0: return br<Z, 0>();			case 0xD1: return cmpA<&CPU::izy>();
	case 0xD5: return cmpA<&CPU::zpx>();	case 0xD6: return DEC<&CPU::zpx>();
	case 0xD8: return flags<D, 0>();		case 0xD9: return cmpA<&CPU::aby>();
	case 0xDD: return cmpA<&CPU::abx>();	case 0xDE: return DEC<&CPU::_abx>();
	case 0xE0: return cmpX<&CPU::imm>();	case 0xE1: return SBC<&CPU::izx>();
	case 0xE4: return cmpX<&CPU::zp>();		case 0xE5: return SBC<&CPU::zp>();
	case 0xE6: return INC<&CPU::zp>();		case 0xE8: return incX();
	case 0xE9: return SBC<&CPU::imm>();		case 0xEA: return NOP();
	case 0xEC: return cmpX<&CPU::abs>();	case 0xED: return SBC<&CPU::abs>();
	case 0xEE: return INC<&CPU::abs>();		case 0xF0: return br<Z, 1>();
	case 0xF1: return SBC<&CPU::izy>();		case 0xF5: return SBC<&CPU::zpx>();
	case 0xF6: return INC<&CPU::zpx>();		case 0xF8: return flags<D, 1>();
	case 0xF9: return SBC<&CPU::aby>();		case 0xFD: return SBC<&CPU::abx>();
	case 0xFE: return INC<&CPU::_abx>();
	default:
	{
		std::cout << "Invalid OPcode! PC: " << PC << " OPcode: 0x" << std::hex << (int)rd(PC - 1) << "\n";
		return NOP();
	}
	}
}
