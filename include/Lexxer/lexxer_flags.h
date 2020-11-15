#ifndef _LEXXER_FLAGS_DEFINED
#define _LEXXER_FLAGS_DEFINED

// LexxerTokenRules flags ...
enum	ooltr_flags	
	{
	ooltrf_character_rule=1,	// token consists only of ordered characters
	ooltrf_computed_rule=0,		//   otherwise: token is being computed

	ooltrf_intermediate=2,		// token is complete immediately after recognition
	ooltrf_not_intermediate=0,	//   otherwise: token is complete only if the full token text
								//              (seperated by a 'within' token) is equal

	ooltrf_within=4,			// token can appear within other tokens (like C++ '+')
	ooltrf_seperate=0,			// token can appear only as a unit
	};

// LexxerContext flags
enum	oolctx_flags	
	{
	oolctx_exclusive=1
	};

#endif