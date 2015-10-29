#include "stdafx.h"
#include "Emulator.h"





/*
NAME

insertMemory - inserts location and contents into the emulator.

SYNOPSIS

bool emulator::insertMemory(int a_location, string a_contents);

a_location - the address location to insert the machine code.

a_contents - the six digit machine code instructions.

DESCRIPTION

If a valid location is specified the contents are loaded into the
emulator's memory at that location.  Otherwise no insertion is
performed.

RETURNS

True if insert is successful, false otherwise.

AUTHOR

Charles Snyder
*/
bool emulator::insertMemory(int a_location, string a_contents)
{
	if (a_location >= 0 && a_location <= 9999) {
		m_memory[a_location] = a_contents;
		return true;
	}
	else {
		return false;
	}
}


/*
NAME

runProgram - Runs the emulator from inserted instructions.

SYNOPSIS

void emulator::runProgram(int startLocation)

startLocation - the address where the first instruction is located.

DESCRIPTION

Reads through all the machine code instructions that were inserted into the
emulator. At any point if an error occurs, it is displayed to the screen
and the program will exit.  Otherwise each instruction will be
executed until a halt command is encountered.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::runProgram(int startLocation) {
	int opcode, address;
	string strOpcode, strAddress;
	if (startLocation == -1) {
		cout << "No start location specified" << endl;
		exit(1);
	}

	activeLocation = startLocation;
	for (;;) {
		strOpcode = m_memory[activeLocation].substr(0, 2);
		strAddress = m_memory[activeLocation].substr(2, m_memory[activeLocation].length());
		if (strAddress == "????" || strOpcode == "??") {
			cout << "Invalid Opcode or address" << endl;
			exit(1);
		}
		opcode = atoi(strOpcode.c_str());
		address = atoi(strAddress.c_str());
		if (opcode < 1 || opcode > 13) {
			cout << "Invalid Opcode" << endl;
			exit(1);
		}
		if (address < 0 || address > 9999) {
			cout << "Unable to access memory location" << endl;
			exit(1);
		}
		if (opcode == 13) {
			return;
		}
		ExecuteOpCode(opcode, address);
	}
}



/*
NAME

ExecuteOpCode - Runs the emulator from inserted instructions.

SYNOPSIS

void emulator::ExecuteOpCode(int opcode, int address)

opcode - the numeric opcode value,
address - the address on which to execute the specified opcode command.

DESCRIPTION

Performs particular action based on opcode value.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteOpCode(int opcode, int address) {
	switch (opcode) {
	case 1:
		ExecuteAdd(address);
		break;
	case 2:
		ExecuteSub(address);
		break;
	case 3:
		ExecuteMultiply(address);
		break;
	case 4:
		ExecuteDivide(address);
		break;
	case 5:
		ExecuteLoad(address);
		break;
	case 6:
		ExecuteStore(address);
		break;
	case 7:
		ExecuteRead(address);
		break;
	case 8:
		ExecuteWrite(address);
		break;
	case 9:
		ExecuteBranch(address);
		break;
	case 10:
		ExecuteBranchMinus(address);
		break;
	case 11:
		ExecuteBranchZero(address);
		break;
	case 12:
		ExecuteBranchPositive(address);
		break;
	}
}

/*
NAME

ExecuteAdd - Adds contents of accumulator with contents of address.

SYNOPSIS

void emulator::ExecuteAdd(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Adds contents of accumulator with contents of address and increases
the active location by one so the emulator will read the next instruction.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteAdd(int address) {
	accumulator = accumulator + atoi(m_memory[address].c_str());
	activeLocation++;
}




/*
NAME

ExecuteSub - Subtract contents of accumulator with contents of address.

SYNOPSIS

void emulator::ExecuteSub(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Subtract contents of accumulator with contents of address and increase
the active location by one.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteSub(int address) {
	accumulator = accumulator - atoi(m_memory[address].c_str());
	activeLocation++;
}



/*
NAME

ExecuteMultiply - Multiply contents of accumulator with contents of address.

SYNOPSIS

void emulator::ExecuteMultiply(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Multiply contents of accumulator with contents of address and increase
the active location by one.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteMultiply(int address) {
	accumulator = accumulator * atoi(m_memory[address].c_str());
	activeLocation++;
}



/*
NAME

ExecuteDivide - Divide contents of accumulator with contents of address.

SYNOPSIS

void emulator::ExecuteDivide(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Divide contents of accumulator with contents of address.  If the current value
in address is zero then a divide by zero error occurs.  Also increases the
active location by one.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteDivide(int address) {
	if (atoi(m_memory[address].c_str()) != 0) {
		accumulator = accumulator / atoi(m_memory[address].c_str());
	}
	else {
		cout << "Error, Divide by zero" << endl;
		exit(1);
	}
	activeLocation++;
}



/*
NAME

ExecuteLoad - Load contents of address into the accumulator.

SYNOPSIS

void emulator::ExecuteLoad(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Load contents of address into the accumulator.  If the value stored in the address
is larger than six digits, an error occurs.  Otherwise the load is performed and
the active location increases by one.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteLoad(int address) {
	if (atoi(m_memory[address].c_str()) < -999999 || atoi(m_memory[address].c_str()) > 999999) {
		cout << "Value too large to load into accumulator" << endl;
		exit(1);
	}
	accumulator = atoi(m_memory[address].c_str());
	activeLocation++;
}



/*
NAME

ExecuteStore - Store into address the contents of the accumulator.

SYNOPSIS

void emulator::ExecuteStore(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Store into address the contents of the accumulator.  If the value from the accumulator
is larger than six digits, it will not store in memory and an error occurs.  Otherwise
it is stored and the active location increases by one.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteStore(int address) {
	stringstream intToString;
	if (accumulator < -999999 || accumulator > 999999) {
		cout << "Value too large to store in memory" << endl;
		exit(1);
	}
	intToString << accumulator;
	m_memory[address] = intToString.str();
	activeLocation++;
}



/*
NAME

ExecuteRead - A line is read and its first 6 digits are placed in the specified address.

SYNOPSIS

void emulator::ExecuteRead(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

A line is read and its first 6 digits are placed in the specified address. If the input
is not a digit the function returns without increase the active location so the operation
will be performed again. Once valid input is entered it is stored in memory and the
active location is increased by one.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteRead(int address) {
	cout << "? ";
	string input;
	cin >> input;
	for (int i = 0; i < (int)input.length(); i++) {
		if (isdigit(input[i]) == 0 && input[i] != '-') {
			cout << "Invalid input" << endl;
			return;
		}
	}
	input = input.substr(0, 6);

	m_memory[address] = input;
	activeLocation++;
}



/*
NAME

ExecuteWrite - Contents of address are displayed to the console.

SYNOPSIS

void emulator::ExecuteWrite(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Contents of address are displayed to the console and active location is
increased by one.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteWrite(int address) {
	cout << m_memory[address] << endl;
	activeLocation++;
}



/*
NAME

ExecuteBranch - Go to address for next instruction.

SYNOPSIS

void emulator::ExecuteBranch(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Go to address for next instruction.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteBranch(int address) {
	activeLocation = address;
}



/*
NAME

ExecuteBranchMinus - Go to address if contents of accumulator are less than zero.

SYNOPSIS

void emulator::ExecuteBranchMinus(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Go to address if contents of accumulator are less than zero, if it is not
the active location is increased by one.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteBranchMinus(int address) {
	if (accumulator < 0) {
		activeLocation = address;
	}
	else {
		activeLocation++;
	}
}



/*
NAME

ExecuteBranchZero - Go to address if contents of accumulator are equal to zero.

SYNOPSIS

void emulator::ExecuteBranchZero(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Go to address if contents of accumulator are equal to zero.  If they are not
then the active location increases by one.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteBranchZero(int address) {
	if (accumulator == 0) {
		activeLocation = address;
	}
	else {
		activeLocation++;
	}
}



/*
NAME

ExecuteBranchPositive - Go to address if contents of accumulator are greater than zero.

SYNOPSIS

void emulator::ExecuteBranchPositive(int address)

address - the address on which to execute the specified opcode command.

DESCRIPTION

Go to address if contents of accumulator are greater than zero.  If they are not
then the active location increases by one.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void emulator::ExecuteBranchPositive(int address) {
	if (accumulator > 0) {
		activeLocation = address;
	}
	else {
		activeLocation++;
	}
}