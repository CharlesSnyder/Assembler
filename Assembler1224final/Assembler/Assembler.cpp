//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.
Assembler::Assembler(int argc, char *argv[])
: m_facc(argc, argv)
{
	// Nothing else to do here.
}
// Destructor currently does nothing.
Assembler::~Assembler()
{
}


/*
NAME

PassI - establishes the location of the labels.

SYNOPSIS

void Assembler::PassI( );

DESCRIPTION

The function will read in a line from the file access object.  It will then
parse the line and determine the type of instruction.  If it is a comment,
the next line will be read.  If it is the end command the function returns.
Otherwise it checks to see if the line contained a label and if so adds that
label to the symbol table.  The next location is then calculated and the
process continues until the end command is encountered.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Assembler::PassI()
{
	int loc = 0;        // Tracks the location of the instructions to be generated.
	int lineCount = 0;  // Keeps track of the current line from the file for error processing.

	// Successively process each line of source code.
	for (;;) {
		
		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {
			
			// If there are no more lines, we are missing an end statement.
			return;
		}

		// Increase the line count for error checking.
		lineCount++;

		// Parse the line and get the instruction type.
		m_inst.setLineCount(lineCount);
		Instruction::InstructionType st = m_inst.RecordInstruction(buff);

		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) return;

		// Labels can only be on machine language and assembler language
		// instructions.
		if (st == Instruction::ST_Comment) continue;

		// If the instruction has a label, record it and its location in the
		// symbol table.
		if (m_inst.isLabel()) {

			if (m_symtab.AddSymbol(m_inst.GetLabel(), loc) == false) {
				string error = "Symbol already in table";
				Errors::RecordError(lineCount, error);
			}
		}
		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
}



/*
NAME

PassII - translates the assembly instructions into machine code, displays original
	     and translated contents, and also displays any errors.

SYNOPSIS

void Assembler::PassII( );

DESCRIPTION

The function will read in a line from the file access object.  It will be similar
to Pass I except now if the instruction is determined to be assembly or machine type
a symbol is looked up from the table if necessary, the machine code is calculated, and
the translation is printed to the screen.  Following this the contents (or machine code)
are inserted into the emulator along with the current location value.  The next location is
calculated and any errors that were encountered for the current line are printed to
the screen.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Assembler::PassII() {
	m_facc.rewind();  // Go back to beginning of file to read through again.
	int loc = 0;        // Tracks the location of the instructions to be generated.
	int lineCount = 0;  // Keeps track of the current line from the file for error processing.
	
	// Display column headers.
	cout << endl;
	cout << "Translation of Program:" << endl << endl;
	cout << "Location" << "   " << "Contents" << "   " << "Original Statement" << endl;

	bool endFound = false; // Flag to signal whether an end command has been encountered.

	// Successively process each line of source code.
	for (;;) {

		// Read the next line from the source file.
		string buff;
		string contents = "";
		
		if (!m_facc.GetNextLine(buff)) {
			// Increase the line count for error checking.
			lineCount++;
			// If there are no more lines, we are missing an end statement.
			if (endFound == false) {
				string error = "No end statement";
				Errors::RecordError(lineCount, error);
			}
			Errors::DisplayErrors(lineCount);
			return;
		}

		// Increase the line count for error checking.
		lineCount++;

		// Parse the line and get the instruction type.
		m_inst.setLineCount(lineCount);
		Instruction::InstructionType st = m_inst.RecordInstruction(buff);

		
		if (st == Instruction::ST_Comment) {
			cout << "                      " << m_inst.GetOriginalInstruction() << endl;
			continue;
		}

		// Check to see if end command is last line.
		else if (st == Instruction::ST_End) {
			endFound = true;
			if (m_facc.isEndLine() == false) {
				cout << "                    " << m_inst.GetOriginalInstruction() << endl;
				continue;
			}
			else {
				cout << "                    " << m_inst.GetOriginalInstruction() << endl;
				return;
			}
		}

		//  Instruction type was assembly or machine type.
		else {

			// If end command was encountered any following lines are errors.
			if (endFound == true) {
				string error = "Line after end statement";
				Errors::RecordError(lineCount, error);
			}

			// Display location value.
			cout << "  " << loc << "      ";
			int symbolLocation; // Holds the location for a potential symbol operand.
			string invalidSymbol; // Will hold ???? if an error occurs.
			
			// Determines whether operand is symbol and if so looks it up from the symbol table.
			if (FindSymbol(symbolLocation, invalidSymbol) == false) {
				string error = "Undefined label";
				Errors::RecordError(lineCount, error);
			}
			
			// Calculates the machine code translation.
			contents = CalculateContents(symbolLocation, invalidSymbol);

			// Prints the machine code contents and original instruction to screen.
			m_inst.PrintTranslation(contents);
		}

		// Load the machine code into the emulator.
		if (m_inst.GetOpCode() != "DS" && m_inst.GetOpCode() != "ORG") {
			if (LoadIntoEmulator(loc, contents) == false) {
				string error = "Attempted to write to invalid memory location";
				Errors::RecordError(lineCount, error);
			}
		}
		
		if (loc > 9999 || loc < 0) {
			string error = "Invalid Memory Location";
			Errors::RecordError(lineCount, error);
		}
		// Determine next memory location.
		loc = m_inst.LocationNextInstruction(loc);

		// Display any errors for the current line.
		Errors::DisplayErrors(lineCount);
		
	}
}


/*
NAME

RunEmulator - Runs the emulator.

SYNOPSIS

void Assembler::RunEmulator( );

DESCRIPTION

Runs the emulator and displays the results to the screen.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Assembler::RunEmulator() {
	cout << endl;
	cout << "Results from emulating program:" << endl << endl;
	m_emul.runProgram(m_inst.GetStartLocation());
	cout << endl << "End of emulation" << endl;
}



/*
NAME

LoadIntoEmulator - Loads machine code and location into the emulator.

SYNOPSIS

bool Assembler::LoadIntoEmulator( int location, string contents );

int location - the location of the current instruction in memory.
string contents - the six digit machine code translation stored
			      as a string for better error detection.

DESCRIPTION

Inserts the location and contents into emulator memory.

RETURNS

True if insertion was valid, false if an error occured.

AUTHOR

Charles Snyder
*/
bool Assembler::LoadIntoEmulator(int location, string contents) {
	if (m_emul.insertMemory(location, contents) == false) {
		return false;
	}
	else {
		return true;
	}
}

/*
NAME

CalculateContents - Determines the machine code from the opcode and operand values.

SYNOPSIS

string Assembler::CalculateContents(int symbolLocation, string invalidSymbol);

int symbolLocation - holds the location where the symbol is located in memory.
string invalidSymbol - tells the function whether an error occured in symbol lookup.

DESCRIPTION

Based on particular opcode and operand combinations particular values will
get placed into the contents stringstream.  The stream will be converted
to a string and returned.

RETURNS

contents as a string.  It will be a six digit long string (if no errors occured).

AUTHOR

Charles Snyder
*/
string Assembler::CalculateContents(int symbolLocation, string invalidSymbol) {
	stringstream contents; // Stream to hold the machine code contents.

	// If the opcode is DC then the contents are just the operand, no opcode is needed in the calculation.
	if (m_inst.GetOpCode() == "DC") {
		contents << setw(6) << setfill('0') << m_inst.GetOperand();
		return contents.str();
	}

	// If the opcode is DS or ORG then there are no contents.
	else if (m_inst.GetOpCode() == "DS" || m_inst.GetOpCode() == "ORG") {
		return "";
	}

	// If the opcode is a machine instruction first set the 2 lefthand digits of the machine code.
	else if (m_inst.GetNumericOpCode() > 0) {
		contents << setw(2) << setfill('0') << m_inst.GetNumericOpCode() << setw(4) << setfill('0');
	}

	// This is if an error was encountered and the current opcode value is "??".
	else {
		contents << setw(2) << setfill('0') << m_inst.GetOpCode() << setw(4) << setfill('0');
	}

	// This section is for a machine instructions address section of the contents.
	// If a symbol was looked up and valid, store that location.
	if (symbolLocation != -1 && invalidSymbol == "") {
		contents << symbolLocation;
	}
	// If invalid symbol was set to "????", set that as address portion of the contents.
	else if (invalidSymbol != "") {
		contents << invalidSymbol;
	}
	// This is if the operand was not a symbol.
	else if (m_inst.GetOpCode() != "HALT" && m_inst.GetOpCode() != "END") {
		contents << m_inst.GetOperandValue();
	}
	// Default case load in zero for address.
	else {
		contents << 0;
	}

	return contents.str();
}


/*
NAME

FindSymbol - Determines whether operand is a symbol and if so looks up its location
			 from the symbol table.

SYNOPSIS

bool Assembler::FindSymbol(int &symbolLocation, string &invalidSymbol);

int symbolLocation - holds the location where the symbol is located in memory, passed by reference.
string invalidSymbol - tells the function whether an error occured in symbol lookup, passed by reference.

DESCRIPTION

First step is to determine whether or not the operand is a number or symbol.  If it is a symbol a lookup
occurs.  If this lookup finds a valid value symbolLocation will hold that value and invalidSymbol will not
get set.  If at any point an error occurs invalidSymbol will get set to "????".

RETURNS

True if Lookup was successful or no lookup was performed, false if lookup is performed and the symbol is not found.

AUTHOR

Charles Snyder
*/
bool Assembler::FindSymbol(int &symbolLocation, string &invalidSymbol) {
	// Determine whether the operand is a symbol or numeric.
	if (m_inst.GetIsNumericOperand() == false && m_inst.GetOpCode() != "HALT" && m_inst.GetOperand() != "????") {
		// Determined to be a symbol, perform the lookup.
		if (m_symtab.LookupSymbol(m_inst.GetOperand(), symbolLocation) == false) {
			symbolLocation = 10000;
			invalidSymbol = "????";
			return false;
		}
		return true;
	}
	// Error occured in earlier function, set invalid symbol.
	else if (m_inst.GetOperand() == "????" && m_inst.GetOpCode() != "HALT" && m_inst.GetOpCode() != "END") {
		invalidSymbol = "????";
		symbolLocation = -1;
		return true;
	}
	// Operand was numeric.
	else {
		symbolLocation = -1;
		return true;
	}
}



