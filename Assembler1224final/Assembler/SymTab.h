//
//		Symbol table class.
//
#pragma once



// This class is our symbol table.
class SymbolTable {

public:
	SymbolTable() {}
	~SymbolTable() {}

	// Add a new symbol to the symbol table.
	bool AddSymbol(string &a_symbol, int a_loc);

	// Display the symbol table.
	void DisplaySymbolTable();

	// Lookup a symbol in the symbol table.
	bool LookupSymbol(string &a_symbol, int &a_loc);

private:

	// This is the actual symbol table.  The symbol is the key to the map.
	map<string, int> m_symbolTable;

};