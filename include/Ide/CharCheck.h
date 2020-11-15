/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CCharCheck by Thomas Hauth (ThomasHauth@gmx.de)

A MFC Class which offers you the abbility to check Strings for invalid characters
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Some time ago, I wrote an application which allows the user to type in his name. To prevent the user from typing "dirty" characters like §)"§)"=$ I wrote the CCharCheck class. 

You can tell this class exactly which characters are allowed. Then, CCharCheck checks your strings for invalid chars and tells you about it.
You add single chars by calling AddChar(...). The following piece of code adds the character 'a': 

// Adding the character 'a'
myCharCheck.AddChar( 'a' )

You can also add ranges to the list of allowed characters. For example you want to add all which lie in the ASCII table between a and z. So you call AddRange( 'a', 'z' ). All characters betwenn a and z are added. NOTE: the 2 chars you give (in our example a and z) are also added. It is not possible to do this operation the other direction (AddRange('z', 'a')). NOTE: CCharCheck works case sensitiv!

For example if you want to add all capital and minor letters and all numbers, you would type: 

// add all minor and capital letters and all numbers
	CharCheck.AddRange( 'a', 'z' );
	CharCheck.AddRange( 'A', 'Z' );
	CharCheck.AddChar( '0' );
	CharCheck.AddChar( '1' );
	CharCheck.AddChar( '2' );
	CharCheck.AddChar( '3' );
	CharCheck.AddChar( '4' );
	CharCheck.AddChar( '5' );
	CharCheck.AddChar( '6' );
	CharCheck.AddChar( '7' );
	CharCheck.AddChar( '8' );
	CharCheck.AddChar( '9' );

OK, now we've told the class which chracters are allowed. Now the function CheckString is called to check a string for invalid parameters. The first paramter of CheckString is from type CString and it's the string you want to check. The second param gives you the ability to tell CheckString from which position in the string he should start checking. 2 means after the first the char of the string. 0 means the whole string should be searched.
If an not allowed char was found, CheckString returns the position of in the string (1 is at the first position). If nothing was found, 0 is returned. 

The function ResetRanges is used to delete all Ranges and chars which werer added. It's a full reset of the class.

Planed improvements 
I want to add a function which just deletes the not allowed chars from the string. 

Sample Code: 

	CCharCheck CharCheck;
	int ret;

	// add all minor letters and all numbers
	CharCheck.AddRange( 'a', 'z' );
	CharCheck.AddChar( '0' );
	CharCheck.AddChar( '1' );
	CharCheck.AddChar( '2' );
	CharCheck.AddChar( '3' );
	CharCheck.AddChar( '4' );
	CharCheck.AddChar( '5' );
	CharCheck.AddChar( '6' );
	CharCheck.AddChar( '7' );
	CharCheck.AddChar( '8' );
	CharCheck.AddChar( '9' );

	// check the string
	// if return value is bigger than 0, a not allowed char was found in the string
	ret = CharCheck.CheckString( "This is the string to check " , 0 );

	if (ret  > 0)
	{
		// string contains invalid char
	}
	else
	{
		// string OK
	}	

NOTE: This class is free, you can use it for whatever you want. But it would be nice if you send me an EMail and tell me about your experiences (ThomasHauth@gmx.de). 

Thomas Hauth 
*/




#if !defined(AFX_CHARCHECK_H__C5665B00_AAB8_11D4_9F15_0080C8B915F0__INCLUDED_)
#define AFX_CHARCHECK_H__C5665B00_AAB8_11D4_9F15_0080C8B915F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCharCheck  
{
public:
	bool AddChar( char Char );
	int CheckString( CString strToCheck, long lBegin);
	void ResetRanges();
	bool AddRange( char cBegin, char cEnd );
	CCharCheck();
	virtual ~CCharCheck();

private:
	CByteArray Ranges;
	bool IsCharInList( char charToCheck );

};

#endif // !defined(AFX_CHARCHECK_H__C5665B00_AAB8_11D4_9F15_0080C8B915F0__INCLUDED_)
