// C4786: identifier was truncated to '255' characters in the debug information
#pragma warning ( disable : 4786 )

// To compile neatly under warning level 4,
// the following warnings are turned off here:
// C4100 unreferenced formal parameter
// C4663 C++ language change: to explicitly specialize 
//       class template 'xxx' use the following syntax:
// 
// The STL implementation is not _that_ clean :)

#pragma warning( push , 3)
#include <string>
#pragma warning( pop )

#ifdef _UNICODE
namespace std {
	typedef	std::basic_string<wchar_t>			tstring;
	typedef std::basic_stringstream<wchar_t>	tstringstream;
	};
#else
namespace std {
	typedef	std::basic_string<char>				tstring;
	typedef std::basic_stringstream<char>		tstringstream;
	};
#endif

#include "ctkFlagsMixin.h"
#include "ctkMisc.h"
#include "ctkCheckValid.h"
#include "ctkExternalObjectPointer.h"