#include"CPU.h"

int main() {
	CPU* cpu = new CPU(new MemManager());
	cpu->exec();
}