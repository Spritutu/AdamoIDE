// CharCheck.cpp: Implementierung der Klasse CCharCheck.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdafx.h"
#include "CharCheck.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCharCheck::CCharCheck()
{

}

CCharCheck::~CCharCheck()
{

}

bool CCharCheck::AddRange(char cBegin, char cEnd)
{
	char curChar;
	
	if (cEnd > cBegin)
	{
		for (curChar = cBegin; curChar <= cEnd; curChar++)
		{
			// check if this char is in the list
			if (IsCharInList( curChar ) == false)
				Ranges.Add( curChar );
		}

		return true;
	}
	else
		return false;
}

void CCharCheck::ResetRanges()
{
	Ranges.RemoveAll();
}

int CCharCheck::CheckString(CString strToCheck, long lBegin)
{
	long curPos;
	long RangesCounter;
	bool CharOK;

	if (lBegin < 1)
		lBegin = 1;

	for (curPos = lBegin; curPos <= strToCheck.GetLength(); curPos ++)
	{
		// check if this char lies in the ranges
		CharOK = false;
		
		for (RangesCounter = 0; RangesCounter <= Ranges.GetUpperBound(); RangesCounter ++)
		{
			if (Ranges[RangesCounter] == strToCheck.GetAt( curPos -1 ))
				CharOK = true;
		}

		if (!CharOK)
			// char was not found
			return curPos;
	}

	return 0;
}

bool CCharCheck::AddChar(char Char)
{
	if (IsCharInList( Char ) == false)
		Ranges.Add( Char );

	return true;
}


bool CCharCheck::IsCharInList( char charToCheck )
{
	int Counter;

	for(Counter = 0; Counter <= Ranges.GetUpperBound(); Counter++)
	{
		if (Ranges[Counter] == charToCheck)
			return true;
	}


	return false;
}