

 /*Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 *It is provided "as is" without express or implied warranty.
 * kings_oz@yahoo.com
 */

// StackX.h: interface for the CStackX class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TREX_
#define _TREX_


#include <stdlib.h>
#include "ArrayBoundsEx.h"

#define ARRAY_MAX_SIZE 10000

template <class TRex>
class CStackX
{
public:
	CStackX();
	CStackX(long val);
	virtual ~CStackX();
	void push(TRex);
	TRex pop();
	TRex peek();
    int  getcount ();
private:
	ArrayOutOfBounds *ex ;
	void setSize(long);
	long getSize();
	long arr_size;
	TRex *vect ;
	long top;


};

#endif

template <class TRex>
CStackX<TRex>::CStackX()
{
	//call super second constructor with value of 1000

	//create exception
	ex = new ArrayOutOfBounds();
	vect = new TRex[ARRAY_MAX_SIZE];
	top = -1;

}
template <class TRex>
CStackX<TRex>::CStackX(long val)
{
	ex = new ArrayOutOfBounds();
	setSize(val);
	vect = new TRex[arr_size];
	top = -1;

}
template <class TRex>
CStackX<TRex>::~CStackX()
{
	//delete array dynamically allocated
	delete ex;
	delete[] vect;
}
template <class TRex>
void CStackX<TRex>::push(TRex value)
{
   try
   {
		if(top != arr_size)
		{
	   //top +=1;
			vect[++top] = value;
		}
		else
			throw ex;
   }
   catch(ArrayOutOfBounds * arrayExc)
   {
      arrayExc->printError("Max Value of Stack Reached oops.....\n");
	  exit(1);
   }


}

template <class TRex>
TRex CStackX<TRex>::pop()
{
	try
	{
		if(top != -1)
		return vect[top--];
		else
			throw ex;
	}
	catch(ArrayOutOfBounds* exc)
	{
		exc->printError("End of Array  can't pop anymore ...\n");
		exit(1);
	}

}

template <class TRex>
TRex CStackX<TRex>::peek()
{
	try
	{
		if(top != -1)
		return vect[top];
		else
			throw ex;
	}
	catch(ArrayOutOfBounds* exc)
	{
		exc->printError("Peeking time closed nothing to see anymore...\n");
		exit(1);
	}

}

template <class TRex>
long CStackX<TRex>::getSize()
{
  return arr_size;
}

template <class TRex>
void CStackX<TRex>::setSize(long val)
{
	arr_size = val;
}

template <class TRex>
int CStackX<TRex>::getcount()
{
    return top+1;
}
