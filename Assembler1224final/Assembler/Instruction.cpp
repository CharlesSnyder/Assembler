#include "stdafx.h"
#include "Instruction.h"

/*
NAME

RecordInstruction - Parse a line from the the file to determine the the type of
					instruction and record all important values.

SYNOPSIS

Instruction::InstructionType Instruction::RecordInstruction(string &a_buff);

a_buff - passed by reference, the line from the file to be parsed.

DESCRIPTION

First parse out all comments from the line.  Then determine whether or not
a label exists on the line.  Perform some error checking and set all important values including
label, opcode, and operand.  Then determine if the opcode is an assembly instruction or 
machine language instruction.  If assembly check to see if it is an end command.

RETURNS

InstructionType ST_Comment if after parsing the line for comments, it is blank.
ST_End if the opcode is determined to be "END".
ST_AssemblerInstr if it is an assembly instruction with no associated numeric opcode.
Otherwise it is a ST_MachineLanguage.

AUTHOR

Charles Snyder
*/
Instruction::InstructionType Instruction::RecordInstruction(string &a_buff)
{
	m_instruction = a_buff;  // Store the original instruction.
	m_Label = "";  // Reset the label.
	m_IsNumericOperand = false; // Reset the numeric operand flag.

	// Parse out any comments
	int pos = (int)a_buff.find_first_of(';');
	string noCommentString = a_buff.substr(0, pos);
	trimBlanks(noCommentString);

	string label, opcode, operand, error;

	// Read in the contents of the line.
	istringstream parseLine(noCommentString);

	// The line was not a comment.
	if (!noCommentString.empty()) {
		// Check to see if a label exists on the line.
		if (noCommentString[0] != ' ' && noCommentString[0] != '\t') {
			parseLine >> label >> opcode >> operand >> error;
			makeUpperCase(opcode);
			SetLabel(label);
			SetOpCode(opcode);
			SetOperand(operand);

			FindBlankAndGarbageValues(opcode, operand, error, true);

			if (opcode == "ORG" || opcode == "HALT" || opcode == "END") {
				string error = "Command should not have label";
				Errors::RecordError(lineCount, error);
			}

			if (m_IsNumericOperand == true) {
				SetNumOperandValue(operand);
			}

			if (isAssemblerInstruction(opcode) == true) {
				AssemblyLabelError();
				m_type = ST_AssemblerInstr;
				return ST_AssemblerInstr;
			}
			else {
				MachineNoSymbolOperandError();
				DetermineNumOpCode(opcode);
				m_type = ST_MachineLanguage;
				return ST_MachineLanguage;
			}
		}
		// Line contained no symbol value.
		else {
			parseLine >> opcode >> operand >> error;
			makeUpperCase(opcode);
			SetOpCode(opcode);
			SetOperand(operand);

			if (opcode == "DS" || opcode == "DC") {
				string error = "Missing symbol";
				Errors::RecordError(lineCount, error);
			}

			if (opcode == "HALT" || opcode == "END") {
				if (!operand.empty()) {
					string errorMessage = "Operand after halt or end";
					Errors::RecordError(lineCount, errorMessage);
				}
				if (opcode == "END") {
					m_type = ST_End;
					return ST_End;
				}
			}

			FindBlankAndGarbageValues(opcode, operand, error, false);

			if (m_IsNumericOperand == true) {
				SetNumOperandValue(operand);
				if (opcode == "ORG") {
					startLocation = m_OperandValue;
				}
			}
			if (isAssemblerInstruction(opcode) == true) {
				AssemblyLabelError();
				m_type = ST_AssemblerInstr;
				return ST_AssemblerInstr;
			}
			else {
				MachineNoSymbolOperandError();
				DetermineNumOpCode(opcode);
				m_type = ST_MachineLanguage;
				return ST_MachineLanguage;
			}
		}
	}
	// The line was a comment.
	else {
		m_type = ST_Comment;
		return ST_Comment;
	}
}





/*
NAME

trimBlanks - Trims the trailing blanks from a string.

SYNOPSIS

void Instruction::trimBlanks(string &a_str);

a_str - passed by reference, the string to have its trailing spaces removed.

DESCRIPTION

Trims the trailing blanks from a string.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Instruction::trimBlanks(string &a_str) {
	string space = "";
	regex findSpace("\\s+$");

	string result = regex_replace(a_str, findSpace, space);
	a_str = result;
}



/*
NAME

FindBlankAndGarbageValues - Perform error checking on line parameters.

SYNOPSIS

void Instruction::FindBlankAndGarbageValues(string opcode, string operand, string overflow, bool labelFlag);

opcode - opcode value from instruction.

operand - operand value from instruction.

overflow - any value following the operand from instruction.

labelFlag - bool value set to true if label is on the line, false otherwise.

DESCRIPTION

Checks each parameter value to ensure that is is not empty.  Also checks to see if any
values exist following the operand which would be considered an error.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Instruction::FindBlankAndGarbageValues(string opcode, string operand, string overflow, bool labelFlag) {
	if (!overflow.empty()) {
		string errorMessage = "Too many fields";
		Errors::RecordError(lineCount, errorMessage);
	}
	if (labelFlag == true) {
		if (opcode == "") {
			string errorMessage = "Missing opcode";
			Errors::RecordError(lineCount, errorMessage);
		}
		if (operand == "") {
			string errorMessage = "Missing operand";
			Errors::RecordError(lineCount, errorMessage);
		}
	}
	else {
		if (operand == "" && opcode != "HALT") {
			string errorMessage = "Missing operand";
			Errors::RecordError(lineCount, errorMessage);
		}
	}
}



/*
NAME

AssemblyLabelError - see whether an assembly instruction has a label.

SYNOPSIS

void Instruction::AssemblyLabelError()

DESCRIPTION

Error check on assembly instructions to ensure that no label is
on the line.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Instruction::AssemblyLabelError() {
	if (m_OpCode == "DC" && m_IsNumericOperand == false) {
		string error = "DC opcode has symbol operand";
		Errors::RecordError(lineCount, error);
		m_Operand = "????";
	}
	else if (m_OpCode == "DS" && m_IsNumericOperand == false) {
		string error = "DS opcode has symbol operand";
		Errors::RecordError(lineCount, error);
		m_Operand = "????";
	}
	else if (m_OpCode == "ORG" && m_IsNumericOperand == false) {
		string error = "ORG opcode has symbol operand";
		Errors::RecordError(lineCount, error);
		m_Operand = "????";
	}
}



/*
NAME

MachineNoSymbolOperandError - see whether machine instruction has a symbolic operand.

SYNOPSIS

void Instruction::MachineNoSymbolOperandError()

DESCRIPTION

Error check on machine instruction to ensure it has a symbolic operand.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Instruction::MachineNoSymbolOperandError() {
	if (m_IsNumericOperand == true) {
		string error = "OpCode does not have symbolic operand";
		Errors::RecordError(lineCount, error);
		m_Operand = "????";
	}
}



/*
NAME

makeUpperCase - make a word all uppercase letters.

SYNOPSIS

void Instruction::makeUpperCase(string &word)

word - passed by reference, the string to transformed into all uppercase letters.

DESCRIPTION

Cycles through length of word and makes each character uppercase one at a time.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Instruction::makeUpperCase(string &word) {
	for (int i = 0; i < (int)word.length(); i++) {
		word[i] = toupper(word[i]);
	}
}



/*
NAME

isValidLabel - ensure the label value meets specified requirements.

SYNOPSIS

void Instruction::isValidLabel(string label)

label - the label value parsed from the line.

DESCRIPTION

Checks to see that label is at most 10 characters long and also does not start with a number.

RETURNS

True if no error detected, false otherwise.

AUTHOR

Charles Snyder
*/
bool Instruction::isValidLabel(string label) {
	if (label.length() > 10 || isalpha(label[0]) == 0) {
		string error = "Invalid label";
		Errors::RecordError(lineCount, error);
		return false;
	}
	else {
		return true;
	}
}



/*
NAME

isValidOperand - ensure the operand value meets specified requirements.

SYNOPSIS

void Instruction::isValidOperand(string operand)

operand - the operand value parsed from the line.

DESCRIPTION

Checks to see that if the operand is a symbol, it is at most 10 characters long
and does not start with a number.  If it is a number ensure that it can be at
most 4 digits.

RETURNS

True if no error detected, false otherwise.

AUTHOR

Charles Snyder
*/
bool Instruction::isValidOperand(string operand) {
	if (operand == "") {
		return false;
	}
	if (DetermineNumericOperand(operand) == false) {
		if (isalpha(operand[0]) == 0 || operand.length() > 10) {
			string error = "Invalid operand";
			Errors::RecordError(lineCount, error);
			return false;
		}
		else {
			return true;
		}
	}
	else {
		if (operand.length() <= 4) {
			return true;
		}
		else {
			return false;
		}
	}
}



/*
NAME

isValidOpCode - ensure the opcode value meets specified requirements.

SYNOPSIS

void Instruction::isValidOpCode(string opcode)

opcode - the opcode value parsed from the line.

DESCRIPTION

Checks to see that the opcode is equal to one of the values existing in the language library.

RETURNS

True if no error detected, false otherwise.

AUTHOR

Charles Snyder
*/
bool Instruction::isValidOpCode(string opcode) {
	string opCodeArray[17] = { "ADD", "SUB", "MULT", "DIV", "LOAD", "STORE", "READ", "WRITE", "B", "BM", "BZ", "BP", "HALT", "ORG", "DC", "DS", "END" };
	makeUpperCase(opcode);
	for (int i = 0; i < 17; i++) {
		if (opcode == opCodeArray[i]) {
			return true;
		}
	}
	string error = "Invalid OpCode";
	Errors::RecordError(lineCount, error);
	return false;
}




/*
NAME

LocationNextInstruction - compute the location of the next instruction.

SYNOPSIS

int Instruction::LocationNextInstruction(int a_loc);

a_loc - the current location value.

DESCRIPTION

Determines the next location value based on the current opcode.

RETURNS

a_loc - the newly calculated location value.

AUTHOR

Charles Snyder
*/
int Instruction::LocationNextInstruction(int a_loc) {
	if (m_OpCode == "ORG") {
		a_loc = m_OperandValue;
	}
	else if (m_OpCode == "DS") {
		a_loc = a_loc + m_OperandValue;
	}
	else {
		a_loc++;
	}
	return a_loc;
}


/*
NAME

DetermineNumericOperand - Check whether the operand is a symbol or number.

SYNOPSIS

bool Instruction::DetermineNumericOperand(string operand);

operand - the operand value to be determined if it is a symbol or number.

DESCRIPTION

Iterate through the operand character by character and check to see that each
one is a digit.

RETURNS

True if the operand is a number, false if it is a symbol.

AUTHOR

Charles Snyder
*/
bool Instruction::DetermineNumericOperand(string operand) {
	if (operand == "") {
		return false;
	}
	for (int i = 0; i < (int)operand.length(); i++) {
		if (isdigit(operand[0]) == 0) {
			m_IsNumericOperand = false;
			return false;
		}
	}
	m_IsNumericOperand = true;
	return true;
}



/*
NAME

DetermineNumOpCode - Set the numeric op code value based on the opcode from the instruction.

SYNOPSIS

void Instruction::DetermineNumOpCode(string opcode);

opcode - the opcode from the line.

DESCRIPTION

Based on the opcode set a numeric value to m_NumOpCode.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Instruction::DetermineNumOpCode(string opcode) {
	makeUpperCase(opcode);
	if (opcode == "ADD") {
		m_NumOpCode = 1;
	}
	else if (opcode == "SUB") {
		m_NumOpCode = 2;
	}
	else if (opcode == "MULT") {
		m_NumOpCode = 3;
	}
	else if (opcode == "DIV") {
		m_NumOpCode = 4;
	}
	else if (opcode == "LOAD") {
		m_NumOpCode = 5;
	}
	else if (opcode == "STORE") {
		m_NumOpCode = 6;
	}
	else if (opcode == "READ") {
		m_NumOpCode = 7;
	}
	else if (opcode == "WRITE") {
		m_NumOpCode = 8;
	}
	else if (opcode == "B") {
		m_NumOpCode = 9;
	}
	else if (opcode == "BM") {
		m_NumOpCode = 10;
	}
	else if (opcode == "BZ") {
		m_NumOpCode = 11;
	}
	else if (opcode == "BP") {
		m_NumOpCode = 12;
	}
	else if (opcode == "HALT") {
		m_NumOpCode = 13;
	}
	else {
		m_NumOpCode = 0;
	}
}




/*
NAME

isAssemblerInstruction - determines whether the opcode is an assembler instruction.

SYNOPSIS

bool Instruction::isAssemblerInstruction(string opcode)

opcode - the opcode from the line.

DESCRIPTION

Determine if the opcode matches any of the assembler instructions.

RETURNS

True if the opcode does match one of the assembler instructions, false otherwise.

AUTHOR

Charles Snyder
*/
bool Instruction::isAssemblerInstruction(string opcode) {
	makeUpperCase(opcode);
	if (opcode == "ORG" || opcode == "END" || opcode == "DC" || opcode == "DS") {
		return true;
	}
	else {
		return false;
	}
}


/*
NAME

PrintTranslation - Prints the machine code and original instructions to the screen.

SYNOPSIS

void Instruction::PrintTranslation(string contents)

contents - the machine code contents.

DESCRIPTION

Print the contents and original instruction to the screen for various
opcode scenarios.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Instruction::PrintTranslation(string contents) {
	if (m_OpCode == "DC") {
		cout << contents << "     " << m_instruction << endl;
	}
	else if (m_OpCode == "DS" || m_OpCode == "ORG") {
		cout << "           " << m_instruction << endl;
	}
	else {
		cout << contents << "     " << m_instruction << endl;
	}
}



/*
NAME

SetLabel - Set the m_Label variable.

SYNOPSIS

void Instruction::SetLabel(string a_label)

a_label - new label value.

DESCRIPTION

Set the m_Label member variable.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Instruction::SetLabel(string a_label) {
	if (isValidLabel(a_label) == true) {
		m_Label = a_label;
	}
	else {
		m_Label = "????";
	}
}


/*
NAME

SetOpCode - Set the m_OpCode variable.

SYNOPSIS

void Instruction::SetOpCode(string a_opcode)

a_opcode - new opcode value.

DESCRIPTION

Set the m_OpCode member variable.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Instruction::SetOpCode(string a_opcode) {
	if (isValidOpCode(a_opcode) == true) {
		m_OpCode = a_opcode;
	}
	else {
		m_OpCode = "??";
	}
}


/*
NAME

SetOperand - Set the m_Label variable.

SYNOPSIS

void Instruction::SetOperand(string a_operand)

a_operand - new operand value.

DESCRIPTION

Set the m_Operand member variable.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Instruction::SetOperand(string a_operand) {
	if (isValidOperand(a_operand) == true) {
		m_Operand = a_operand;
	}
	else {
		m_Operand = "????";
	}
}