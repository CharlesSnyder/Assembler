//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"


class Assembler {

public:
	Assembler(int argc, char *argv[]);
	~Assembler();

	// Pass I - establish the locations of the symbols
	void PassI();

	// Pass II - generate a translation
	//at the end of pass 2 feed in location and content to emulator.
	void PassII();

	// Display the symbols in the symbol table.
	void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }

	// Run emulator on the translation.
	void RunEmulator();

	//Load contents into the emulator
	bool LoadIntoEmulator(int location, string contents);

	//Calculate the machine code instructions
	string CalculateContents(int symbolLocation, string invalidSymbol);

	//Find symbol from symbol table if necessary
	bool FindSymbol(int &symbolLocation, string &invalidSymbol);

private:

	FileAccess m_facc;	    // File Access object
	SymbolTable m_symtab;	// Symbol table object
	Instruction m_inst;	    // Instruction object
	emulator m_emul;        // Emulator for VC3600
};
