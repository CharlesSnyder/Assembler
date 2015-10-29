#include "stdafx.h"
#include "Errors.h"



/*
NAME

InitErrorReporting - clears the contents of the error multimap.

SYNOPSIS

void Errors::InitErrorReporting();

DESCRIPTION

Clear the contents of the error multimap.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Errors::InitErrorReporting() {
	m_ErrorMsgs.clear();
}



/*
NAME

RecordError - record an error into the error multimap.

SYNOPSIS

void Errors::RecordError(int line, string &a_emsg);

line - the line in the file where the error occured.

a_emsg - passed by reference, the message to indicate what the error is.

DESCRIPTION

Inserts an error based on the line and message.  The multimap allows for
different error messages to be recorded for the same line.  Each time
an error is being recorded a check is made to see whether or not it is
already in the map.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Errors::RecordError(int line, string &a_emsg) {
	int count = 0;
	if (m_ErrorMsgs.empty() == true) {
		m_ErrorMsgs.insert(pair<int, string>(line, a_emsg));
	}
	else {
		bool found = false;
		for (multimap<int, string>::iterator errorIterator = m_ErrorMsgs.find(line); count < (int)m_ErrorMsgs.count(line); count++, errorIterator++) {
			if (errorIterator->second == a_emsg) {
				found = true;
			}
		}
		if (found == false) {
			m_ErrorMsgs.insert(pair<int, string>(line, a_emsg));
		}
	}
}




/*
NAME

DisplayErrors - display the contents of error multimap for a given line.

SYNOPSIS

void Errors::DisplayErrors(int line);

line - the line in the file where the error occured.

DESCRIPTION

If there are no errors for the line the function does nothing; otherwise
it iterates through the multimap displaying to the console all errors
that are associated with the particular line key value.

RETURNS

Nothing.

AUTHOR

Charles Snyder
*/
void Errors::DisplayErrors(int line) {
	if (m_ErrorMsgs.count(line) != 0) {
		int count = 0;
		for (multimap<int, string>::iterator errorIterator = m_ErrorMsgs.find(line); count < (int)m_ErrorMsgs.count(line); count++, errorIterator++) {
			cerr << errorIterator->second << ", ";
		}
		cout << endl;
	}
}

multimap<int, string> Errors::m_ErrorMsgs;