/*Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 *It is provided "as is" without express or implied warranty.
 * kings_oz@yahoo.com
 */

// ArrayOutOfBounds interface for the ArrayOutOfBounds class.
//
//////////////////////////////////////////////////////////////////////



#ifndef _ARRAYSOUT_
#define _ARRAYSOUT_

class ArrayOutOfBounds
{
public:
	ArrayOutOfBounds() { }
	virtual ~ArrayOutOfBounds() {}
	void printError (char* str) { printf(str); }
};

#endif
