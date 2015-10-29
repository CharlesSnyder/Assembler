//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/*
NAME

FileAccess - Constructor for FileAccess class.

SYNOPSIS

FileAccess::FileAccess(int argc, char *argv[]);

argc - the number of command line arguments.

argv - the commmand line arguments in an array.

DESCRIPTION

Opens the file specified from the command line arguments.  

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
FileAccess::FileAccess(int argc, char *argv[])
{
	// Check that there is exactly one run time parameter.
	if (argc != 2) {
		cerr << "Usage: Assem <FileName>" << endl;
		exit(1);
	}
	// Open the file.
	m_sfile.open(argv[1]);

	// If the open failed, report the error and terminate.
	if (!m_sfile) {
		cerr << "Source file could not be opened, assembler terminated."
			<< endl;
		exit(1);
	}
}

/*
NAME

~FileAccess - Destructor for FileAccess class.

SYNOPSIS

FileAccess::~FileAccess();

DESCRIPTION

Closes the file specified from the command line arguments.  

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
FileAccess::~FileAccess()
{
	m_sfile.close();
}


/*
NAME

GetNextLine - Get the next line from the file.

SYNOPSIS

bool FileAccess::GetNextLine(string &a_buff)

a_buff - passed by reference, the next line retrieved from the file.

DESCRIPTION

Reads the next line from the file and places it in the a_buff
variable.

RETURNS

True if a new line was read, false if there were no more lines in the file.

AUTHOR

Charles Snyder
*/
bool FileAccess::GetNextLine(string &a_buff)
{
	if (m_sfile.eof()) return false;

	getline(m_sfile, a_buff);

	// Return indicating success.
	return true;
}


/*
NAME

rewind - Move file back to beginning for processing.

SYNOPSIS

void FileAccess::rewind();

DESCRIPTION

Clears end of file flag and then seeks back to the start of the file in
order to read it again.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void FileAccess::rewind()
{
	// Clean the end of file flag and go back to the beginning of the file.
	m_sfile.clear();
	m_sfile.seekg(0, ios::beg);
}



/*
NAME

isEndLine - Checks to see if the file has reached its end.

SYNOPSIS

bool FileAccess::isEndLine();

DESCRIPTION

Checks the end of file flag and returns a bool value based on whether
or not it is set.

RETURNS

True if the end of file flag is set, false if it is NOT set.

AUTHOR

Charles Snyder
*/
bool FileAccess::isEndLine() {
	if (!m_sfile.eof()) {
		return false;
	}
	else {
		return true;
	}
}