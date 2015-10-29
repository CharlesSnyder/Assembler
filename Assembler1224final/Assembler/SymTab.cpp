//
//		Implementation of the symbol table class.
//
#include "stdafx.h"
#include "SymTab.h"

/*
NAME

AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

bool SymbolTable::AddSymbol(string &a_symbol, int a_loc)

a_symbol - string passed by reference, the symbol to add.

a_loc - the location to associate with the specified symbol.

DESCRIPTION

Place a_symbol into the symbol value table along with its associated
location a_loc.

RETURNS

True if the add is successful, false if the symbol already exists in the table.

AUTHOR

Charles Snyder
*/
bool SymbolTable::AddSymbol(string &a_symbol, int a_loc)
{
	// If the symbol is already in the symbol table, record it as multiply defined.
	map<string, int>::iterator st;
	st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end()) {
		st->second = -999;
		return false;
	}
	// Record a the  location in the symbol table.
	m_symbolTable[a_symbol] = a_loc;
	return true;
}




/*
NAME

DisplaySymbolTable - displays the contents of the symbol table to the screen.

SYNOPSIS

void SymbolTable::DisplaySymbolTable();

DESCRIPTION

Iterates through the symbol table and outputs each symbol and its associated location.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void SymbolTable::DisplaySymbolTable()
{
	cout << "Symbol Table:" << endl << endl;
	cout << "Symbol #" << "     " << "Symbol" << "     " << "Location" << endl;
	int count = 0;
	for (map<string, int>::iterator symbolIterator = m_symbolTable.begin(); symbolIterator != m_symbolTable.end(); ++symbolIterator)
	{
		cout << setw(4) << count << setw(14) << symbolIterator->first << setw(10) << symbolIterator->second << endl;
		count++;
	}
}






/*
NAME

LookUpSymbol - looks up a specified symbol from the symbol table.

SYNOPSIS

bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc);

a_symbol - passed by reference, the symbol to look up in the table.

a_loc - passed by reference, the location associated with the found symbol.

DESCRIPTION

Checks to see if a specified symbol exists in the symbol table and if so
returns its location.

RETURNS

True if the symbol exists in the table and is found, false if the symbol
does NOT exist in the table.

AUTHOR

Charles Snyder
*/
bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc) {
	if (m_symbolTable.count(a_symbol) != 0) {
		a_loc = m_symbolTable[a_symbol];
		return true;
	}
	else {
		return false;
	}
}