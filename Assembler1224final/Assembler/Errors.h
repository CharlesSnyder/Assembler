//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#pragma once

#include <string>
#include <map>

class Errors {

public:

	// Initializes error reports.
	static void InitErrorReporting();

	// Records an error message.
	static void RecordError(int line, string &a_emsg);

	// Displays the collected error message.
	static void DisplayErrors(int line);

private:

	// Multimap to store multiple errors per line.
	static multimap<int, string> m_ErrorMsgs;
};