//
//		Emulator class - supports the emulation of VC3600 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class emulator {

public:

	const static int MEMSZ = 10000;	// The size of the memory of the VC3600.
	emulator() {}

	// Records instructions and data into VC3600 memory.
	bool insertMemory(int a_location, string a_contents);

	// Runs the VC3600 program recorded in memory.
	void runProgram(int startLocation);

private:

	// The memory of the VC3600.
	string m_memory[MEMSZ];

	// The current location to process.
	int activeLocation;

	// Extra word to hold values for mathematical operations.
	int accumulator;

	// Determines which action to perform based on OpCode.
	void ExecuteOpCode(int opcode, int address);

	// Adds contents of accumulator with contents of address.
	void ExecuteAdd(int address);

	// Subtract contents of accumulator with contents of address.
	void ExecuteSub(int address);

	// Multiply contents of accumulator with contents of address.
	void ExecuteMultiply(int address);

	// Divide contents of accumulator with contents of address.
	void ExecuteDivide(int address);

	// Load contents of address into the accumulator.
	void ExecuteLoad(int address);

	// Store into address the contents of the accumulator.
	void ExecuteStore(int address);

	// A line is read and its first 6 digits are placed in the specified address.
	void ExecuteRead(int address);

	// Contents of address are displayed to the console.
	void ExecuteWrite(int address);

	// Go to address for next instruction.
	void ExecuteBranch(int address);

	// Go to address if contents of accumulator are less than zero.
	void ExecuteBranchMinus(int address);

	// Go to address if contents of accumulator are equal to zero.
	void ExecuteBranchZero(int address);

	// Go to address if contents of accumulator are greater than zero.
	void ExecuteBranchPositive(int address);
};

#endif
