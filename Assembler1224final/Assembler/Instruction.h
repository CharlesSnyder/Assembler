//
// Class to manage instructions
//
#pragma once

#include "Errors.h"
using namespace std;

// The elements of an instruction.
class Instruction {

public:

	Instruction() { startLocation = -1; };
	~Instruction() { };

	// Codes to indicate the type of instruction we are processing.
	enum InstructionType {
		ST_MachineLanguage, // A machine language instruction.
		ST_AssemblerInstr,  // Assembler Language instruction. DC DS, etc
		ST_Comment,          // Comment or blank line
		ST_End                    // end instruction.
	};

	// Record and parse the Instruction.
	InstructionType RecordInstruction(string &a_buff);

	// Trims trailing blanks from a string.
	void trimBlanks(string &a_str);

	// Sets error if any values are missing or too many values are on a line.
	void FindBlankAndGarbageValues(string opcode, string operand, string overflow, bool labelFlag);

	//  Sets error if assembly instruction has symbol operand.
	void AssemblyLabelError();

	// Sets error if machine instruction does NOT have symbol operand.
	void MachineNoSymbolOperandError();

	// Sets a word to all uppercase letters.
	void makeUpperCase(string &word);

	// Checks whether a label meets specified requirements.
	bool isValidLabel(string label);

	// Checks whether the operand meets specified requirements.
	bool isValidOperand(string operand);

	// Checks whether the opcode meets specified requirements.
	bool isValidOpCode(string opcode);

	// Compute the location of the next instruction.
	int LocationNextInstruction(int a_loc);

	// To access the label
	inline string &GetLabel() {

		return m_Label;
	};

	// To determine if a label is blank.
	inline bool isLabel() {

		return !m_Label.empty();
	};

	// Determine whether the operand is a number or symbol.
	bool DetermineNumericOperand(string operand);



	// Determine and set the numeric op code.
	void DetermineNumOpCode(string opcode);

	// Determine whether the current instruction is assembly or machine instruction.
	bool isAssemblerInstruction(string opcode);

	// Set the line count value.
	inline void setLineCount(int lc) {
		if (lc >= 0) {
			lineCount = lc;
		}
	}

	// Print the machine code and original instruction to the console.
	void PrintTranslation(string contents);


	inline void SetNumOperandValue(string operand) {
		int numVal = atoi(operand.c_str());
		m_OperandValue = numVal;
	};

	inline string GetOriginalInstruction() {
		return m_instruction;
	};

	inline bool GetIsNumericOperand() {
		return m_IsNumericOperand;
	};

	inline string GetOperand() {
		 return m_Operand;
	};

	inline string GetOpCode() {
		return m_OpCode;
	};

	inline int GetNumericOpCode() {
		return m_NumOpCode;
	};

	inline int GetOperandValue() {
		return m_OperandValue;
	};

	inline int GetStartLocation() {
		return startLocation;
	};

	void SetLabel(string a_label);

	void SetOpCode(string a_opcode);

	void SetOperand(string a_operand);

private:


	// The elemements of a instruction
	string m_Label;            // The label.
	string m_OpCode;       // The symbolic op code.
	string m_Operand;      // The operand.


	string m_instruction;    // The original instruction.

	// Derived values.
	int m_NumOpCode;     // The numerical value of the op code.
	InstructionType m_type; // The type of instruction.

	//org ds dc
	bool m_IsNumericOperand;// == true if the operand is numeric.
	int m_OperandValue;   // The value of the operand if it is numeric.

	int lineCount;
	int startLocation;

};

