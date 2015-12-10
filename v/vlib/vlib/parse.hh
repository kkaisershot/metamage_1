/*
	parse.hh
	--------
*/

#ifndef VLIB_PARSE_HH
#define VLIB_PARSE_HH

// vlib
#include "vlib/value.hh"


namespace vlib
{
	
	Value parse( const char* p );
	
	Value eval_tree( const Value& tree );
	
}

#endif